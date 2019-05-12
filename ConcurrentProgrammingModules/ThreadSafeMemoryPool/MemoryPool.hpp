#include "MemoryPool.h"


using namespace Module;


template <typename T, const uint32_t PoolSize, class Ctor>
MemoryPool<T, PoolSize, Ctor>::MemoryPool()
	:_pFreeMemoryEntries(nullptr), _pFreeUnusedDatas(nullptr)
{
	_pFreeMemoryEntries = AllocatedList();
	_pFreeUnusedDatas = AllocatedList();

	Reserve();
}


template <typename T, const uint32_t PoolSize, class Ctor>
MemoryPool<T, PoolSize, Ctor>::~MemoryPool()
{
	if (_pFreeMemoryEntries)
	{
		FreeList(_pFreeMemoryEntries);
		_pFreeMemoryEntries = nullptr;
	}

	if (_pFreeUnusedDatas)
	{
		FreeList(_pFreeUnusedDatas);
		_pFreeUnusedDatas = nullptr;
	}
}

template <typename T, const uint32_t PoolSize, class Ctor>
T* MemoryPool<T, PoolSize, Ctor>::Allocate()
{
	PSLIST_ENTRY listEntry = ::InterlockedPopEntrySList(_pFreeUnusedDatas);

	if (listEntry == nullptr)
	{
		_Reserve(std::max<uint32_t>(1, PoolSize / 5));
		return _constructor.Construct();
	}

	MemoryEntry* const pEntry = reinterpret_cast<MemoryEntry*>(listEntry);
	T* const pData = pEntry->pData;

	pEntry->pData = nullptr;
	::InterlockedPushEntrySList(_pFreeMemoryEntries, listEntry);

	return pData;
}

template <typename T, const uint32_t PoolSize, class Ctor>
void MemoryPool<T, PoolSize, Ctor>::Free(T* const ptr)
{
	PSLIST_ENTRY pListEntry = ::InterlockedPopEntrySList(_pFreeMemoryEntries);

	if (pListEntry == nullptr)
	{
		pListEntry = static_cast<PSLIST_ENTRY>(::_aligned_malloc(sizeof(MemoryEntry),
			MEMORY_ALLOCATION_ALIGNMENT));

		if (pListEntry == nullptr)
		{
			_constructor.Destruct(ptr);
			return;
		}

	}

	MemoryEntry* const pEntry = reinterpret_cast<MemoryEntry*>(pListEntry);
	pEntry->pData = ptr;

	::InterlockedPushEntrySList(_pFreeUnusedDatas, &pEntry->ItemEntry);
}

template <typename T, const uint32_t PoolSize, class Ctor>
PSLIST_HEADER MemoryPool<T, PoolSize, Ctor>::AllocatedList()
{
	PSLIST_HEADER pListHeader = static_cast<PSLIST_HEADER>(
		::_aligned_malloc(sizeof(SLIST_HEADER), MEMORY_ALLOCATION_ALIGNMENT));

	if (pListHeader == nullptr)
		return nullptr;

	::InitializeSListHead(pListHeader);
	return pListHeader;
}

template <typename T, const uint32_t PoolSize, class Ctor>
void MemoryPool<T, PoolSize, Ctor>::FreeList(PSLIST_HEADER& header)
{
	PSLIST_ENTRY pListEntry = ::InterlockedFlushSList(header);

	if (nullptr != pListEntry)
	{
		MemoryEntry* pEntry = nullptr;

		do {
			pEntry = reinterpret_cast<MemoryEntry*>(pListEntry);
			pListEntry = pListEntry->Next;

			_constructor.Destruct(pEntry->pData);
		} while (nullptr != pListEntry);
	}


	if (nullptr != header)
	{
		::_aligned_free(header);

		header = nullptr;
	}
}

template <typename T, const uint32_t PoolSize, class Ctor>
void MemoryPool<T, PoolSize, Ctor>::Reserve(const uint32_t size)
{
	MemoryEntry* pEntry = static_cast<MemoryEntry*>(::_aligned_malloc(
		sizeof(MemoryEntry) * size, MEMORY_ALLOCATION_ALIGNMENT));

	for (auto index = 0; size > index; ++index, ++pEntry)
	{
		pEntry->pData = _constructor.Construct();

		::InterlockedPushEntrySList(_pFreeUnusedDatas, &pEntry->ItemEntry);
	}
}