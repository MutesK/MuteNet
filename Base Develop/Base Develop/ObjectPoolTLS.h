#pragma once

#include <Windows.h>
#include <list>
#include "ObjectPool.h"


#define BLOCK_CHECK 0xABCDEF123456789A
#define INIT_CHECK 0x777778888899999A
template <class DATA>
class CObjectPoolTLS
{
private:
	class CChunkBlock
	{
	private:
#pragma pack(push, 1)
		struct st_ChunkDATA
		{
			unsigned __int64 BlockCheck; // 64비트
			CChunkBlock *pThisChunk;		// 64비트
			DATA	Data;
			CObjectPoolTLS<CChunkBlock>* pMemoryPool;
			bool    Alloced;
		};
#pragma pack(pop)
	public:
		CChunkBlock(CObjectPoolTLS<CChunkBlock> *pMemoryPool, int BlockSize = 2000, bool mConstructor = false)
		{
			Constructor = mConstructor;

			if (pArrayChunk == nullptr)
				pArrayChunk = (st_ChunkDATA *)malloc(sizeof(st_ChunkDATA) * BlockSize);

			for (int i = 0; i < BlockSize; i++)
			{
				pArrayChunk[i].BlockCheck = BLOCK_CHECK;
				pArrayChunk[i].pThisChunk = this;
				pArrayChunk[i].Alloced = false;
				pArrayChunk[i].pMemoryPool = pMemoryPool;
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


			if (Constructor)
				new (ret) DATA();

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

			if (InterlockedDecrement(&m_lReferenceCount) == 0)
			{
				pBlock->pMemoryPool->Free(pBlock->pThisChunk);
				//free(pArrayChunk);
				m_lInit = INIT_CHECK;
			}

			if (Constructor)
				pData->~DATA();

			return true;
		}

	private:
		st_ChunkDATA *pArrayChunk;

		LONG	m_lReferenceCount;
		LONG	m_lAllocCount;
		bool    Constructor;
		LONG64	m_lInit; // 재사용



		template <class DATA>
		friend class CMemoryPoolTLS;
	};

public:
	CObjectPoolTLS(int ChunkSize = 1000, int BlockSize = 0, bool mConstructor = false)
		:BlockSize(BlockSize), b_Constructor(mConstructor), ChunkSize(ChunkSize)
	{
		m_lAllocCount = 0;

		pMemoryPool = new CMemoryPool<CChunkBlock>(BlockSize, false);

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
			pBlock = pMemoryPool->Alloc();
			new (pBlock) CChunkBlock(pMemoryPool, ChunkSize, b_Constructor);
			TlsSetValue(TLSIndex, pBlock);
		}

		DATA* pRet = pBlock->Alloc();
		InterlockedAdd(&m_lAllocCount, 1);


		if (pBlock->m_lAllocCount == ChunkSize || pBlock->m_lReferenceCount == 0)
			TlsSetValue(TLSIndex, nullptr);

		return pRet;
	}
	bool Free(DATA *pData)
	{
		CChunkBlock::st_ChunkDATA *pBlock = (CChunkBlock::st_ChunkDATA *)((__int64 *)pData - 2);


		if (pBlock->pThisChunk->Free(pData, pBlock))
			InterlockedAdd(&m_lAllocCount, -1);

		return true;
	}
	long GetChunkSize()
	{
		return pMemoryPool->GetAllocCount();
	}
	long GetAllocCount()
	{
		return  max(m_lAllocCount, 0);
	}
private:
	CObjectPoolTLS<CChunkBlock>* pMemoryPool;

	DWORD TLSIndex;
	LONG	m_lAllocCount;
	int		BlockSize;
	int		ChunkSize;

	bool	b_Constructor;
};

