#pragma once

#include <Windows.h>
#include <new>
#include <vector>
#include <list>
using namespace std;

template <class DATA>
class CObjectPool
{
private:
	friend class ObjectPoolTLS;
	friend class CChunkBlock;

	struct st_BLOCK_NODE
	{
		DATA Data;
		st_BLOCK_NODE *pNextBlock;
	};

	struct st_Pop
	{
		st_BLOCK_NODE *_TopNode;
		__int64		   UniqueCount;
	};
public:
	//////////////////////////////////
	// 생성자
	// int - 블럭 갯수
	// bool - 블록 생성자 호출여부(기본값 = FALSE)
	//////////////////////////////////
	CObjectPool(int blockSize = 50000);
	virtual ~CObjectPool();


	//////////////////////////////////
	// 블록 하나를 할당해주는 함수 -> new 선언해줘야 한다면 한다.
	// 리턴 : 특정 블록의 공간 포인터 리턴
	//////////////////////////////////
	DATA* Alloc(void);

	//////////////////////////////////
	// 사용중인 블록을 반환하는 함수
	// 파라미터 : 사용중인 데이터 주소값-> 소멸자 호출해야 된다면 하고 안한다면 그냥 반환
	// 리턴 : 성공여부
	//////////////////////////////////
	bool Free(DATA *pData); // 그렇다면 외부에서 이 함수를 통해 반환하고, 나중에 이 주소값을 사용하려고 한다면? -> 주의


	//////////////////////////////////
	// 총 확보된 블록의 갯수 리턴
	//////////////////////////////////
	int GetBlockCount(void);



	//////////////////////////////////////////////////////////////////////////
	// 현재 사용중인 블럭 개수를 얻는다.
	//
	// 파라미터: 사용중인 블럭 개수.
	//////////////////////////////////////////////////////////////////////////
	int GetAllocCount(void);


private:
	void MakeNewNode();
private:
	// 생성시 할당량
	volatile LONG m_iBlockSize;

	volatile LONG m_iAllocCount;
	volatile LONG64 m_iUnique;

	volatile st_Pop *pPop;
	volatile st_BLOCK_NODE *pTail;

	volatile std::list<st_BLOCK_NODE *> List;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class DATA>
CObjectPool<DATA>::CObjectPool(int blockSize)
{
	m_iAllocCount = 0;
	m_iBlockSize = 0;
	st_BLOCK_NODE *pNewNode = nullptr;
	st_BLOCK_NODE *pOldNode = nullptr;

	pNewNode = (st_BLOCK_NODE *)malloc(sizeof(st_BLOCK_NODE));
	memset(pNewNode, 0, sizeof(st_BLOCK_NODE));
	memset(&pNewNode->Data, 0, sizeof(DATA));
	List.push_back(pNewNode);

	pPop = (st_Pop *)_aligned_malloc(128, 16);
	pPop->_TopNode = pNewNode;
	pPop->UniqueCount = 0;
	pOldNode = pNewNode;


	for (int i = 1; i <= blockSize; i++)
	{
		pNewNode = (st_BLOCK_NODE *)malloc(sizeof(st_BLOCK_NODE));
		pOldNode->pNextBlock = pNewNode;
		pNewNode->pNextBlock = nullptr;
		memset(&pNewNode->Data, 0, sizeof(DATA));

		List.push_back(pNewNode);

		pOldNode = pNewNode;
		m_iBlockSize++;
	}

	pTail = pNewNode;
}

template <class DATA>
CObjectPool<DATA>::~CObjectPool()
{
	auto iter = List.begin();

	for (; iter != List.end();)
	{
		st_BLOCK_NODE *pNode = (*iter);
		List.erase(iter);
		free(pNode);
	}

	_aligned_free(pPop);

}

template <class DATA>
DATA* CObjectPool<DATA>::Alloc(void)
{
	st_Pop OldPop;
	st_BLOCK_NODE *pNewNode = nullptr;
	DATA *ret = nullptr;
	size_t UniqueCount = InterlockedIncrement64(&m_iUnique) % MAXLONG64;

RETRY:
	if (pPop->_TopNode->pNextBlock == nullptr)
	{	
		for(int i=0; i<10000; i++)
			MakeNewNode();
	}

	while (1)
	{
		OldPop = { 0 };

		// 자료구조를 변경한다.
		OldPop._TopNode = pPop->_TopNode;
		OldPop.UniqueCount = pPop->UniqueCount;

		if (OldPop._TopNode->pNextBlock == nullptr)
			goto RETRY;

		pNewNode = OldPop._TopNode->pNextBlock;

		if (InterlockedCompareExchange128((LONG64 *)pPop, (size_t)UniqueCount, (size_t)pNewNode, (LONG64 *)&OldPop))
		{
			ret = &OldPop._TopNode->Data;
			break;
		}

	}
	InterlockedIncrement(&m_iAllocCount);

	return ret;
}

template <class DATA>
bool CObjectPool<DATA>::Free(DATA *pData)
{
	st_BLOCK_NODE *pDel = (st_BLOCK_NODE *)(pData);

	if (InterlockedAdd(&m_iAllocCount, -1) < 0)
	{
		InterlockedAdd(&m_iAllocCount, 1);
		return false;
	}

	st_Pop OldPop;
	LONG64 UniqueCount = InterlockedIncrement64(&m_iUnique) % MAXLONG64;
	while (1)
	{
		OldPop = { 0 };

		OldPop._TopNode = pPop->_TopNode;
		OldPop.UniqueCount = pPop->UniqueCount;

		if (OldPop._TopNode != pDel->pNextBlock && OldPop._TopNode != pDel)
			pDel->pNextBlock = OldPop._TopNode;

		if (InterlockedCompareExchange128((LONG64 *)pPop, (size_t)UniqueCount, (size_t)pDel, (LONG64 *)&OldPop))
			break;
	}

	return true;
}


template <class DATA>
void CObjectPool<DATA>::MakeNewNode()
{
	st_BLOCK_NODE *pNewNode = (st_BLOCK_NODE *)malloc(sizeof(st_BLOCK_NODE));
	memset(pNewNode, 0, sizeof(st_BLOCK_NODE));
	pNewNode->pNextBlock = nullptr;

	List.push_back(pNewNode);

	st_Pop OldPop; 
	size_t UniqueCount = InterlockedIncrement64(&m_iUnique) % MAXLONG64;

	while (1)
	{
		OldPop = { 0 };

		OldPop._TopNode = pPop->_TopNode;
		OldPop.UniqueCount = pPop->UniqueCount;

		if (OldPop._TopNode != pNewNode->pNextBlock && OldPop._TopNode != pNewNode)
			pNewNode->pNextBlock = OldPop._TopNode;
		else continue;

		if (InterlockedCompareExchange128((LONG64 *)pPop, (size_t)UniqueCount, (size_t)pNewNode, (LONG64 *)&OldPop))
		{
			InterlockedAdd(&m_iBlockSize, 1);
			break;
		}
	}
}

template <class DATA>
int CObjectPool<DATA>::GetBlockCount(void)
{
	return m_iBlockSize;
}

template <class DATA>
int CObjectPool<DATA>::GetAllocCount(void)
{
	return m_iAllocCount;
}