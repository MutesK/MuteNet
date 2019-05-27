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

class DoublyQueue final
{
public:
	DoublyQueue();
	~DoublyQueue();

	void SwapQueues();

	void Put(const std::string& _data);
	void Get(std::string& OUT _data);
private:
	SafeSharedLock _lock;

	std::queue<std::string> _forwardQueue;
	std::queue<std::string> _backwardQueue;
};

