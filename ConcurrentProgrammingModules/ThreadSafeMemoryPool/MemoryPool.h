#pragma once

#include "../ConcurrentProgrammingModules/CommonHeader.h"

namespace Util
{
	template <typename T>
	struct Constructor
	{
		inline T* Construct(void) const
		{
			return new T;
		}

		inline void Destruct(T* const ptr) const
		{
			delete ptr;
		}
	};

	template <typename T, const uint32_t PoolSize, class Ctor = Constructor<T>>
	class MemoryPool final
	{
	private:
		struct MemoryEntry
		{
			MemoryEntry(void)
				: ItemEntry(), pData(nullptr)
			{
			}

			SLIST_ENTRY ItemEntry;
			T* pData;
		};

		NON_COPYABLE(MemoryPool);
	public:
		MemoryPool();
		~MemoryPool();

		T* Allocate();
		void Free(T* const ptr);

	private:
		PSLIST_HEADER AllocatedList();
		void FreeList(PSLIST_HEADER& header);
		void Reserve(const uint32_t size);

	private:
		const Ctor _constructor;
		PSLIST_HEADER _pFreeMemoryEntries;
		PSLIST_HEADER _pFreeUnusedDatas;

	};

}