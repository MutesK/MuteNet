#pragma once


#include "ObjectPoolTLS.h"

template <class DATA>
class CLockFreeQueue
{
private:
	struct st_NODE
	{
		DATA Data;
		st_NODE *pNextNode;
	};

	struct st_DUMMY
	{
		st_NODE *pNode;
		__int64	 UniqueCount;
	};
public:
	CLockFreeQueue(int iSize=100000);
	virtual ~CLockFreeQueue();

	void Enqueue(const DATA& data);
	bool Dequeue(DATA *pOutData);


	///////////////////////////////////////////////////
	// ť ���θ� �����ϰ� �����.
	///////////////////////////////////////////////////
	void ClearQueue();
	///////////////////////////////////////////////////
	// ���� ť���� ����� ũ�⸦ �����´�.
	///////////////////////////////////////////////////
	LONG  GetUseSize();
	///////////////////////////////////////////////////
	// ���� ť ��� ����� �����´�.
	///////////////////////////////////////////////////
	int GetQueueSize();

private:
	CObjectPool<st_NODE> *pMemoryPool;

	st_DUMMY *_Rear;
	st_DUMMY *_Front;

	std::atomic<size_t> m_iUseSize;
	std::atomic<size_t> m_iUnique;
};

template <class DATA>
CLockFreeQueue<DATA>::CLockFreeQueue(int iSize)
{
	pMemoryPool = new CObjectPool<st_NODE>(iSize);

	m_iUseSize = m_iUnique = 0;

	_Rear = (st_DUMMY *)_aligned_malloc(128, 16);
	_Rear->pNode = nullptr;
	_Rear->UniqueCount = 0;

	_Front = (st_DUMMY *)_aligned_malloc(128, 16);
	_Front->pNode = nullptr;
	_Front->UniqueCount = 0;

	_Rear->pNode = pMemoryPool->Alloc();

	st_NODE *pNewNode = _Rear->pNode;
	pNewNode->pNextNode = nullptr;

	_Front->pNode = pNewNode;
}

template <class DATA>
CLockFreeQueue<DATA>::~CLockFreeQueue()
{
	_aligned_free(_Rear);
	_aligned_free(_Front);

	ClearQueue();
}
template <class DATA>
void CLockFreeQueue<DATA>::ClearQueue()
{
	while (_Front->pNode != nullptr)
	{
		pMemoryPool->Free(_Front->pNode);
		_Front->pNode = _Front->pNode->pNextNode;
	}

	_Rear->pNode = pMemoryPool->Alloc();

	st_NODE *pNewNode = _Rear->pNode;
	pNewNode->pNextNode = nullptr;

	_Front->pNode = pNewNode;

	m_iUseSize = 0;
}
template <class DATA>
LONG CLockFreeQueue<DATA>::GetUseSize()
{
	return m_iUseSize;
}
template <class DATA>
int CLockFreeQueue<DATA>::GetQueueSize()
{
	return pMemoryPool->GetBlockCount();
}
template <class DATA>
void CLockFreeQueue<DATA>::Enqueue(const DATA& data)
{
	st_NODE *pNewNode = pMemoryPool->Alloc();

	pNewNode->Data = data;
	pNewNode->pNextNode = nullptr;

	st_DUMMY rear;
	st_NODE *pNext;
	LONG64 Unique = (++m_iUnique) % MAXDWORD64;

	while (1)
	{
		rear.pNode = _Rear->pNode;
		rear.UniqueCount = _Rear->UniqueCount;

		pNext = rear.pNode->pNextNode;

		if (pNewNode == rear.pNode)
		{
			if(pNext != nullptr)
				InterlockedCompareExchangePointer((PVOID *)&_Rear->pNode, pNext, rear.pNode);
		}
		
		if (rear.pNode == _Rear->pNode)
		{
			if (pNext == nullptr)
			{
				if (InterlockedCompareExchangePointer((PVOID *)&rear.pNode->pNextNode, pNewNode, nullptr) == nullptr)
				{
					InterlockedCompareExchange128((LONG64 *)_Rear, (LONG64)Unique, (LONG64)pNewNode, (LONG64 *)&rear);
					break;
				}
			}
			else
			{
				InterlockedCompareExchangePointer((PVOID *)&_Rear->pNode, pNext, rear.pNode);
			}
		}

		rear = { 0 };
		pNext = nullptr;
	}

	++m_iUseSize;
}

template <class DATA>
bool CLockFreeQueue<DATA>::Dequeue(DATA *pOutData)
{
	if (m_iUseSize - 1 < 0)
		return false;
	

	LONG64 Unique = (++m_iUnique) % MAXDWORD64;
	st_DUMMY front;
	st_DUMMY rear;
	st_NODE *pNext;

	while (1)
	{
		// Tail�� �ʿ��� ������ ��Ƽ������ ȯ�濡�� ������ ������ ��ġ�� �Ǵ��ϱ� ���ؼ���.
		front.pNode = _Front->pNode;
		front.UniqueCount = _Front->UniqueCount;

		rear.pNode = _Rear->pNode;
		rear.UniqueCount = _Rear->UniqueCount;

		pNext = front.pNode->pNextNode;

		if (pNext == nullptr)
			return false;

		st_NODE *pRearNext = rear.pNode->pNextNode;

		if (pRearNext != nullptr)
		{
			InterlockedCompareExchangePointer((PVOID *)&_Rear->pNode, pRearNext, rear.pNode);
			continue;
		}

		if (front.pNode == _Front->pNode)
		{
			if (front.pNode == rear.pNode || 
				front.pNode == _Rear->pNode)
 			{

				if (pNext == nullptr)
					return false;		

				if (m_iUseSize == 0)
					return false;
			}
			else
			{
				// �����͸� ����.
				*pOutData = pNext->Data;
				
				if (front.pNode == rear.pNode || front.pNode == _Rear->pNode)
					continue;

				if (InterlockedCompareExchange128((LONG64 *)_Front, (LONG64)Unique, (LONG64)pNext, (LONG64 *)&front))
				{
					pMemoryPool->Free(front.pNode);
					break;
				}
			}

		}
	}

	--m_iUseSize;

	return true;
}
