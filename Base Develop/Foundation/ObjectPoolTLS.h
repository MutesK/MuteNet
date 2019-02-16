#pragma once

#include "foundation.h"
#include "ObjectPool.h"

// dynamic TLS Alloc 을 사용하여 스레드별로 ChunkBlock 를 할당하며
// 이를통해서 메모리를 할당 받음.
//
// - CMemoryPool : 통합 메모리 관리자 (CChunkBlock 을 관리함)
// - CChunkBlock : 실제 사용자용 객체를 관리하는 스레드별 할당 블록
//
//
// 1. MemoryPool 인스턴스가 생성되면 CChunkBlock 을 일정량 확보하여 관리 준비에 들어감.
// 2. MemoryPool 생성자에서 TlsAlloc 을 통해서 메모리풀 전용 TLS 인덱스를 확보.
// 3. 이 TLS 인덱스 영역에 각 스레드용 CChunkBlock 이 담겨진다.
//    만약 한 프로세스에서 여러개의 MemoryPool 을 사용하게 된다면 각자의 TLS Index 를 가지게 됨.
// 4. 스레드에서 MemoryPool.Alloc 호출
// 5. TLS Index 에 CChunkBlock 이 있는지 확인.
// 6. 청크블록이 없다면 (NULL 이라면)  MemoryPool 에서 ChunkBlock 을 할당하여 TLS 에 등록.
// 7. TLS 에 등록된 ChunkBlock 에서 DATA Alloc 후 반환.
// 8. ChunkBlock 에 더 이상 DATA 가 없다면 TLS 를 NULL 로 바꾸어 놓고 종료.
//
// 1. MemoryPool.Free 호출시
// 2. 입력 포인터를 기준으로 ChunkBlock 에 접근.
// 3. ChunkBlock Ref 를 줄임.
// 4. ChunkBlock Ref 가 0 이라면 MemoryPool 로 ChunkBlock 반환.
// -. Free 의 경우는 스레드의 구분이 없으므로 스레드 세이프 하도록 구현.
//
// Alloc 할당은 TLS 에 할당된 CChunkBlock 에서 스레드마다 개별적으로 진행
// Free 해제는 CChunkBlock 레퍼런스 카운터만 줄인 뒤 해당 청크블록에 사용중인 개수가 0 인경우 CMemoryPool 로 반환.
//
// ChunkBlock 은 DATA 의 할당만 가능하며, 반환 후 재사용은 고려하지 않음.
// 이유는 반환의 경우 어떤 스레드에서든 가능해야 하기 때문에 동기화 문제로 반환은 없음.
//
// ChunkBlock 에 10 개의 DATA 가 있다면 이 10개의 DATA 를 모두 할당 후 반환 하게 될 때 
// ChunkBlock 자체가 통합 메모리풀로 반환되어 청크 자체의 재 사용을 기다리게 된다.
//////////////////////////////////////////////////////////////////






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


		std::shared_ptr<DATA> pRet(ptr, [&](DATA *ptr)
		{
			this->Free(ptr);
		});
		::new (ptr) DATA(std::forward<Args>(args)...);

		return pRet;
	}

	std::shared_ptr<DATA> get_shared()
	{
		DATA *ptr = Alloc();

		std::shared_ptr<DATA> pRet(ptr, [&](DATA *ptr)
		{
			this->Free(ptr);
		});
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

		std::unique_ptr<DATA> pRet(ptr, [&](DATA *ptr)
		{
			this->Free(ptr);
		});
		::new (ptr) DATA(std::forward<Args>(args)...);

		return pRet;
	}

	std::unique_ptr<DATA> get_unique()
	{
		DATA *ptr = Alloc();
		auto deleter = [&]()
		{
			this->Free(ptr);
		};

		std::unique_ptr<DATA> pRet(ptr, [&](DATA *ptr)
		{
			this->Free(ptr);
		});
		::new (ptr) DATA();

		return pRet;
	}

	template <class... Args>
	std::weak_ptr<DATA> get_weak(Args&&... args)
	{
		DATA *ptr = Alloc();

		std::weak_ptr<DATA> pRet(ptr, [&](DATA *ptr)
		{
			this->Free(ptr);
		});
		::new (ptr) DATA(std::forward<Args>(args)...);

		return pRet;
	}

	std::weak_ptr<DATA> get_weak()
	{
		DATA *ptr = Alloc();

		std::weak_ptr<DATA> pRet(ptr, [&](DATA *ptr)
		{
			this->Free(ptr);
		});
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


