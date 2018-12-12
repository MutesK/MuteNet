#pragma once

#include "foundation.h"

using namespace std;
/*
Object Pool (FreeList형식)
움직이는 알고리즘 자체는 Stack과 비슷함.
Lock Free 알고리즘의 효율성이 비관적 동시성 제어 기법에 비해 성능 향상을 유도하기 어렵다.
심지어 Lock Free 알고리즘과 Fast Spin Lock 알고리즘 성능 평가에서 Spin Lock을 이기기 약간 어려움.
또한 Lock Free 알고리즘은 버그가 나오면 잡기 힘들다.
잘 쓰지 못할꺼면 안쓰는게 나음.
std::shared_mutex 와 OwnChecking 기능을 합한 SafeMutex을 만들면 도입예정.
사용시 주의사항
* MEMORYPOOL_CALL_CTOR 플래그를 사용중이라면, 기본 생성자는 무조건 존재해야된다.
*/

#define VALIDCODE (0x77777777)

template <class NodeType>
class CObjectPool
{
private:
	struct BLOCKNODE
	{
		NodeType Data;
		uint64_t ValidCode;
		uint32_t allocindex;

		BLOCKNODE(uint32_t allocindex)
		{
			ValidCode = VALIDCODE;
			this->allocindex = allocindex;
		}

		~BLOCKNODE()
		{
			cout << "Bug Detected Shared Pointer Reference Count is Zero\n";
			*((int *)0) = 1;
		}
	};
public:
	CObjectPool(int blockSize = 10000, int maxBlockSize = 10000, bool flagcallctor = false);
	virtual ~CObjectPool();

	NodeType* Alloc(void);
	bool Free(NodeType* Data);

	int GetBlockCount(void);
	int GetAllocCount(void);
private:
	bool _isUsector;

	std::atomic<uint32_t>  _allocCount;
	std::atomic<uint32_t>  _maxBlockSize;
	std::atomic<uint32_t>  _blockSize;

	std::map<uint32_t, BLOCKNODE *>		 _freelist;
	std::stack<uint32_t>				 _freestack;  // 안쓰는 인덱스 모아놓은 자료구조 

	std::mutex				     _lock;
};



template <class NodeType>
CObjectPool<NodeType>::CObjectPool(int blockSize, int maxBlockSize, bool flagcallctor)
{
	_allocCount = 0;
	_maxBlockSize = 0;
	_blockSize = 0;
	_freelist.clear();

	_isUsector = false;

	for (int i = 0; i < blockSize; i++)
	{
		_freelist[i] = static_cast<BLOCKNODE *>(malloc(sizeof(BLOCKNODE)));
		_freelist[i]->allocindex = i;
		_freelist[i]->ValidCode = VALIDCODE;
		_freestack.push(i);
	}

	_isUsector = flagcallctor;
	_maxBlockSize = maxBlockSize;
	_blockSize = blockSize;
}
template <class NodeType>
CObjectPool<NodeType>::~CObjectPool()
{
	for (auto iter = _freelist.begin(); iter != _freelist.end(); ++iter)
	{
		std::free(iter->second);
	}

	_freelist.clear();
}
template <class NodeType>
NodeType* CObjectPool<NodeType>::Alloc(void)
{
	std::lock_guard<std::mutex> guard(_lock);
	if (_allocCount >= _blockSize)
	{
		// 새로 DATA을 만들어야됨.
		if (_maxBlockSize <= _blockSize)
			return nullptr;

		int makedblocksize = 2 * _blockSize;
		
		for (int i = _blockSize; i < makedblocksize; i++)
		{
			_freelist[i] = static_cast<BLOCKNODE *>(malloc(sizeof(BLOCKNODE)));
			_freelist[i]->allocindex = i;
			_freelist[i]->ValidCode = VALIDCODE;
			_freestack.push(i);
		}
		_blockSize = makedblocksize;
	}

	int allocindex = static_cast<int>(_freestack.top());
	_freestack.pop();

	NodeType* ret = &_freelist[allocindex]->Data;
	++_allocCount;


#ifdef MEMORYPOOL_CALL_CTOR
	if (_isUsector)
	{
		new (ret) NodeType();
	}
#endif

	return ret;
}
template <class NodeType>
bool CObjectPool<NodeType>::Free(NodeType* Data)
{
	std::lock_guard<std::mutex> guard(_lock);

	if (_allocCount <= 0)
		return false;

	// Danger Code
	BLOCKNODE* pBlockNode = reinterpret_cast<BLOCKNODE *>(Data);

	if (pBlockNode->ValidCode != VALIDCODE)
		return false;

	uint32_t allocindex = pBlockNode->allocindex;
	_freestack.push(allocindex);
	--_allocCount;

#ifdef MEMORYPOOL_CALL_CTOR
	if (_isUsector)
	{
		Data->~NodeType();
	}
#endif

	return true;
}

template <class NodeType>
int CObjectPool<NodeType>::GetBlockCount(void)
{
	return _blockSize;
}

template <class NodeType>
int CObjectPool<NodeType>::GetAllocCount(void)
{
	return _allocCount;
}