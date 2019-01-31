#include <iostream>
#include "CLockFreeQueue.h"
#include "CLockFreeStack.h"
#include <thread>
#include <vector>

using namespace std;

CLockFreeStack<size_t> _stack;
CLockFreeQueue<size_t> _queue;
std::vector<std::thread> _testThreadPool;

int main()
{
	for(int i=0; i<10; i++)
		_testThreadPool.push_back(std::thread([&]()
	{
		while (1)
		{
			size_t temp;
			for (int i = 0; i < 10000; i++)
				_queue.Enqueue(i);

			std::this_thread::sleep_for(1s);

			for (int i = 0; i < 10000; i++)
				_queue.Dequeue(&temp);

			std::this_thread::sleep_for(1s);
		}
	}));

	for (int i = 0; i < 10; i++)
	{
		_testThreadPool[i].joinable();
	}

	while (1)
	{
		std::cout << "LockFree Queue 부하 테스트\n";
		std::cout << "Use Queue Size : " << _queue.GetUseSize() << std::endl;
		std::cout << "Total Queue Data Size : " << _queue.GetQueueSize() << std::endl;
		std::this_thread::sleep_for(0.2s);
		system("cls");
	}

	return 0;
}