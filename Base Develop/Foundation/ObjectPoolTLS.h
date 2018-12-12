#pragma once

#include "ObjectPool.h"

/*
ObjectPool을 스레드별로, 할당하게 하면서도, 해제는 어떤 스레드라도 가능하게 한다.

ChunkDATA을 배열로 ChunkBlock이 가지고있고,
ObjectPool은 ChunkBlock 템플릿 인스턴스화 한다. 

사용시 주의사항
* MEMORYPOOL_CALL_CTOR 플래그를 사용중이라면, 기본 생성자는 무조건 존재해야된다. Placement New을 통해 수동 생성자 호출을 클래스에서 알아서 해주기 때문.

*/

#define BLOCK_CHECK 0xABCDEF123456789A
#define INIT_CHECK 0x777778888899999A

template <class DATA>
class CObjectPoolTLS
{
private:
	class CChunkBlock
	{
	public:
		struct st_ChunkDATA
		{
			DATA	Data;
			uint64_t BlockCheck; 
			CChunkBlock *pThisChunk;
			CObjectPool<CChunkBlock>* ObjectPool;
			bool    Alloced;
		};
	public:
		CChunkBlock()
		{
			m_lReferenceCount = 0;
			m_lAllocCount = 0;
		}
		~CChunkBlock()
		{
			//int *p = nullptr;
			//*p = 0;
		}
		void Init(const std::shared_ptr<CObjectPool<CChunkBlock>>& ObjectPool, int BlockSize = 2000, bool mConstructor = false)
		{
			Constructor = mConstructor;

			if (pArrayChunk == nullptr || m_lInit != INIT_CHECK)
				pArrayChunk = static_cast<st_ChunkDATA *>(malloc(sizeof(st_ChunkDATA) * BlockSize));

			for (int i = 0; i < BlockSize; i++)
			{
				pArrayChunk[i].BlockCheck = BLOCK_CHECK;
				pArrayChunk[i].pThisChunk = this;
				pArrayChunk[i].Alloced = false;
				pArrayChunk[i].ObjectPool = ObjectPool.get();
			}

			m_lInit = 0;
			m_lAllocCount = 0;
			m_lReferenceCount = BlockSize;
		}

		DATA* Alloc()
		{
			if (pArrayChunk[m_lAllocCount].BlockCheck != BLOCK_CHECK)
				return nullptr;

			if (pArrayChunk[m_lAllocCount].Alloced)
				m_lAllocCount++;

			pArrayChunk[m_lAllocCount].Alloced = true;
			DATA *ret = &pArrayChunk[m_lAllocCount].Data;

			m_lAllocCount++;

			return ret;
		}
		bool Free(DATA *pData, st_ChunkDATA *pBlock)
		{
			if (pBlock->BlockCheck != BLOCK_CHECK)
				return false;

			if (!pBlock->Alloced)
				return false;

			pBlock->Alloced = false;
			--m_lReferenceCount;

			if (m_lReferenceCount == 0)
			{
				pBlock->ObjectPool->Free(pBlock->pThisChunk);
				m_lInit = INIT_CHECK;
			}

			return true;
		}

	public:
		st_ChunkDATA *pArrayChunk;

		atomic<uint32_t>	m_lReferenceCount;
		atomic<uint32_t>	m_lAllocCount;
		bool    Constructor;
		atomic<uint64_t>	m_lInit; 


		template <class DATA>
		friend class CMemoryPoolTLS;
	};

public:
	CObjectPoolTLS(int ChunkSize = 1000, int BlockSize = 5000, bool mConstructor = false)
		:BlockSize(BlockSize), b_Constructor(mConstructor), ChunkSize(ChunkSize)
	{
		m_lAllocCount = 0;

		ObjectPool = make_shared<CObjectPool<CChunkBlock>>(ChunkSize, ChunkSize * 2, true);

		TLSIndex = TlsAlloc();
		if (TLSIndex == TLS_OUT_OF_INDEXES)
		{
			int *p = nullptr;
			*p = 0;
		}
	}

	~CObjectPoolTLS()
	{
		TlsFree(TLSIndex);
	}

	DATA* Alloc()
	{
		CChunkBlock *pBlock = (CChunkBlock *)TlsGetValue(TLSIndex);

		if (pBlock == nullptr)
		{
			pBlock = ObjectPool->Alloc();
			pBlock->Init(ObjectPool, BlockSize, b_Constructor);
			TlsSetValue(TLSIndex, pBlock);
		}

		DATA* pRet = pBlock->Alloc();
		new (pRet) DATA();
		++m_lAllocCount;

		if (pBlock->m_lAllocCount == BlockSize || pBlock->m_lReferenceCount == 0)
			TlsSetValue(TLSIndex, nullptr);

		return pRet;
	}
	bool Free(DATA *pData)
	{
		CChunkBlock::st_ChunkDATA *pBlock = reinterpret_cast<CChunkBlock::st_ChunkDATA *>(reinterpret_cast<__int64 *>(pData));

		if (pBlock->pThisChunk->Free(pData, pBlock))
			--m_lAllocCount;

		pData->~DATA();

		return true;
	}
	uint32_t GetChunkSize()
	{
		return ObjectPool->GetAllocCount();
	}
	uint32_t GetAllocCount()
	{
		return  m_lAllocCount;
	}
private:
	std::shared_ptr<CObjectPool<CChunkBlock>> ObjectPool;

	uint32_t TLSIndex;
	static atomic<uint32_t> m_lAllocCount;
	atomic<uint32_t>	BlockSize;
	atomic<uint32_t>	ChunkSize;

	bool	b_Constructor;
};
