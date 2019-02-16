#pragma once

#include "foundation.h"
#include "TypeCAS.h"

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
	// ������
	// int - �� ����
	// bool - ��� ������ ȣ�⿩��(�⺻�� = FALSE)
	//////////////////////////////////
	CObjectPool(int blockSize = 50000);
	virtual ~CObjectPool();


	//////////////////////////////////
	// ��� �ϳ��� �Ҵ����ִ� �Լ� -> new ��������� �Ѵٸ� �Ѵ�.
	// ���� : Ư�� ����� ���� ������ ����
	//////////////////////////////////
	DATA* Alloc(void);

	//////////////////////////////////
	// ������� ����� ��ȯ�ϴ� �Լ�
	// �Ķ���� : ������� ������ �ּҰ�-> �Ҹ��� ȣ���ؾ� �ȴٸ� �ϰ� ���Ѵٸ� �׳� ��ȯ
	// ���� : ��������
	//////////////////////////////////
	bool Free(DATA *pData); // �׷��ٸ� �ܺο��� �� �Լ��� ���� ��ȯ�ϰ�, ���߿� �� �ּҰ��� ����Ϸ��� �Ѵٸ�? -> ����


	//////////////////////////////////
	// �� Ȯ���� ����� ���� ����
	//////////////////////////////////
	int GetBlockCount(void);



	//////////////////////////////////////////////////////////////////////////
	// ���� ������� �� ������ ��´�.
	//
	// �Ķ����: ������� �� ����.
	//////////////////////////////////////////////////////////////////////////
	int GetAllocCount(void);


private:
	void MakeNewNode();
private:
	// ������ �Ҵ緮
	std::atomic<uint32_t> m_iBlockSize;

	std::atomic<uint32_t> m_iAllocCount;
	std::atomic<uint64_t> m_iUnique;

	st_Pop *pPop;
	volatile st_BLOCK_NODE *pTail;

	std::list<st_BLOCK_NODE *> List;
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
		iter = List.erase(iter);
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
	size_t UniqueCount = ++m_iUnique % size_t(std::numeric_limits<uint64_t>::max);

RETRY:
	if (pPop->_TopNode->pNextBlock == nullptr)
	{	
		for(int i=0; i<10000; i++)
			MakeNewNode();
	}

	while (1)
	{
		OldPop = { 0 };

		// �ڷᱸ���� �����Ѵ�.
		OldPop._TopNode = pPop->_TopNode;
		OldPop.UniqueCount = pPop->UniqueCount;

		if (OldPop._TopNode->pNextBlock == nullptr)
			goto RETRY;

		pNewNode = OldPop._TopNode->pNextBlock;

		if (CompareAndSwap128((uint64_t *)pPop, (uint64_t)UniqueCount, (uint64_t)pNewNode, (uint64_t *)&OldPop))
		{
			ret = &OldPop._TopNode->Data;
			break;
		}

	}
	++m_iAllocCount;

	return ret;
}

template <class DATA>
bool CObjectPool<DATA>::Free(DATA *pData)
{
	st_BLOCK_NODE *pDel = (st_BLOCK_NODE *)(pData);

	if (--m_iAllocCount < 0)
	{
		++m_iAllocCount;
		return false;
	}

	st_Pop OldPop;
	uint64_t UniqueCount = ++m_iUnique % size_t(std::numeric_limits<uint64_t>::max);
	while (1)
	{
		OldPop = { 0 };

		OldPop._TopNode = pPop->_TopNode;
		OldPop.UniqueCount = pPop->UniqueCount;

		if (OldPop._TopNode != pDel->pNextBlock && OldPop._TopNode != pDel)
			pDel->pNextBlock = OldPop._TopNode;

		if (CompareAndSwap128((uint64_t *)pPop, (uint64_t)UniqueCount, (uint64_t)pDel, (uint64_t *)&OldPop))
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
	size_t UniqueCount = ++m_iUnique % size_t(std::numeric_limits<uint64_t>::max);

	while (1)
	{
		OldPop = { 0 };

		OldPop._TopNode = pPop->_TopNode;
		OldPop.UniqueCount = pPop->UniqueCount;

		if (OldPop._TopNode != pNewNode->pNextBlock && OldPop._TopNode != pNewNode)
			pNewNode->pNextBlock = OldPop._TopNode;
		else continue;

		if (CompareAndSwap128((uint64_t *)pPop, (uint64_t)UniqueCount, (uint64_t)pNewNode, (uint64_t *)&OldPop))
		{
			++m_iBlockSize;
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