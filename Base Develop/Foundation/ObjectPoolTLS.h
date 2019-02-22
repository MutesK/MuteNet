#pragma once

#include "foundation.h"
#include "ObjectPool.h"



template <class DATA>
class CObjectPoolTLS
{
public:
	class CChunkBlock;
	struct st_ChunkDATA
	{
		DATA	Data;
		CChunkBlock *pThisChunk;
		CObjectPool<CChunkBlock>* pObjectPool;
	};


	class CChunkBlock
	{
	public:
		CChunkBlock(CObjectPool<CChunkBlock> *pObjectPool, int BlockSize = 2000)
		{
			if (pArrayChunk == nullptr)
				pArrayChunk = (st_ChunkDATA *)malloc(sizeof(st_ChunkDATA) * BlockSize);

			for (int i = 0; i < BlockSize; i++)
			{
				pArrayChunk[i].pThisChunk = this;
				pArrayChunk[i].pObjectPool = pObjectPool;
			}

			m_lAllocCount = 0;
			m_lReferenceCount = BlockSize;
		}


		DATA* Alloc()
		{
			DATA *ret = &pArrayChunk[m_lAllocCount].Data;

			m_lAllocCount++;
			return ret;
		}

		bool Free(DATA *pData, st_ChunkDATA *pBlock)
		{
			if (--m_lReferenceCount == 0)
			{
				pBlock->pObjectPool->Free(pBlock->pThisChunk);
			}

			--m_lAllocCount;

			return true;
		}

		friend class CObjectPoolTLS;
	private:
		st_ChunkDATA *pArrayChunk;

		std::atomic<size_t>	m_lReferenceCount;
		std::atomic<size_t>	m_lAllocCount;
	};
public:
	CObjectPoolTLS(int ChunkSize = 1000, int BlockSize = 0)
		:BlockSize(BlockSize), ChunkSize(ChunkSize)
	{
		m_lAllocCount = 0;

		pObjectPool = new CObjectPool<CChunkBlock>(BlockSize);
	}
		
	~CObjectPoolTLS()
	{
	}

	template<class... Args>
	std::shared_ptr<DATA> get_shared(Args&&... args)
	{
		DATA *ptr = Alloc();


		std::shared_ptr<DATA> pRet(ptr, decltype([&](DATA * ptr)
			{
				this->Free(ptr);
			}));
		::new (ptr) DATA(std::forward<Args>(args)...);

		return pRet;
	}

	std::shared_ptr<DATA> get_shared()
	{
		DATA *ptr = Alloc();

		std::shared_ptr<DATA> pRet(ptr, decltype([&](DATA * ptr)
			{
				this->Free(ptr);
			}));
		::new (ptr) DATA();

		return pRet;
	}

	template <class... Args>
	std::unique_ptr<DATA> get_unique(Args&&... args)
	{
		DATA *ptr = Alloc();
		auto deleter = [&]()
		{
			this->Free(ptr);
		};

		std::unique_ptr<DATA> pRet(ptr, decltype([&](DATA *ptr)
		{
			this->Free(ptr);
		}));
		::new (ptr) DATA(std::forward<Args>(args)...);

		return pRet;
	}

	std::unique_ptr<DATA> get_unique()
	{
		DATA *ptr = Alloc();

		std::unique_ptr<DATA> pRet(ptr, decltype([&](DATA * ptr)
			{
				this->Free(ptr);
			}));
		::new (ptr) DATA();

		return pRet;
	}

	template <class... Args>
	std::weak_ptr<DATA> get_weak(Args&&... args)
	{
		DATA *ptr = Alloc();

		std::weak_ptr<DATA> pRet(ptr, decltype([&](DATA * ptr)
			{
				this->Free(ptr);
			}));
		::new (ptr) DATA(std::forward<Args>(args)...);

		return pRet;
	}

	std::weak_ptr<DATA> get_weak()
	{
		DATA *ptr = Alloc();

		std::weak_ptr<DATA> pRet(ptr, decltype([&](DATA * ptr)
			{
				this->Free(ptr);
			}));
		::new (ptr) DATA();

		return pRet;
	}

	long GetChunkSize()
	{
		return pObjectPool->GetAllocCount();
	}
	long GetAllocCount()
	{
		return  m_lAllocCount;
	}
private:
	DATA* Alloc()
	{
		CChunkBlock *pBlock = thread_block;

		if (pBlock == nullptr)
		{
			pBlock = pObjectPool->Alloc();
			new (pBlock) CChunkBlock(pObjectPool, ChunkSize);
			thread_block = pBlock;
		}

		DATA* pRet = (DATA *)pBlock->Alloc();
		++m_lAllocCount;


		if (pBlock->m_lAllocCount == ChunkSize || pBlock->m_lReferenceCount == 0)
			thread_block = nullptr;

		return pRet;
	}

	void Free(DATA *pData)
	{
		st_ChunkDATA *pBlock = reinterpret_cast<st_ChunkDATA *>(pData);

		if (pBlock->pThisChunk->Free(pData, pBlock))
			--m_lAllocCount;

		pData->~DATA();
	}

public:
		std::atomic<size_t>		m_lAllocCount;
		std::atomic<size_t>		BlockSize;
		std::atomic<size_t>		ChunkSize;
private:
	CObjectPool<CChunkBlock>* pObjectPool;
	static thread_local CChunkBlock* thread_block;
};

template <class DATA>
thread_local typename CObjectPoolTLS<DATA>::CChunkBlock* CObjectPoolTLS<DATA>::thread_block = nullptr;


