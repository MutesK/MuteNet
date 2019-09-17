#pragma once

/*
 *	Thread-Safe Double Queue
 *  ���� ��Ҹ� �ִ��� ���̱� ���� ���� 1 ť�� Only Write, 
 *  �Ĺ� ť�� Only Read�Ѵ�.
 *  
 *  ���� ������ �Ǹ� ���� Container�� Swap�ϸ� �̶���, ���� �ɸ���.
 *  Thread A�� Write�� �ϰ�, Thread B�� Read�Ͽ� ó��
 *  
 *  �׽�Ʈ�� ���� Generic���� �ʰ�, std::string ���� �׽�Ʈ�Ѵ�.
 */

#include "../ConcurrentProgrammingModules/CommonHeader.h"
#include "../DeadLock Checker/SafeSharedLock.h"
#include <queue>

template <typename Data>
class DoublyQueue final
{
public:
	DoublyQueue();
	~DoublyQueue();

	void SwapQueues();

	void Put(const Data& _data);
	void Get(Data& OUT _data);
private:
	SafeSharedLock _lock;

	std::queue<Data> _forwardQueue;
	std::queue<Data> _backwardQueue;
};

