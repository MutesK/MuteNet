#include "stdafx.h"
#include "Thread.h"

#include "ObjectPoolTLS.hpp" 
#include "ConsolePrinter.h"
#include "TaskManager.h"
#include <thread>

class Test
{
public:
	int a;

	Test()
	{
		a = 1;
	}
	~Test()
	{
		a = 2;
	}
};

int main()
{
	dolamda function = []()
	{
		CObjectPoolTLS<Test> TestPool(2000, 10000);
		vector<Test *>	   TempStore;
		uint64_t			TestCount = 0;

		while (1)
		{
			for (int i = 0; i < 10000; i++)
			{
				auto variable = TestPool.Alloc();
				if (variable == nullptr)
				{
					*((int *)0) = 1;
				}

				variable->a = i;
				TempStore.push_back(variable);
			}

			for (int i = 0; i < 10000; i++)
			{
				auto b = TempStore[i];

				if (b->a != i)
				{
					*((int *)0) = 1;
				}

				TestPool.Free(b);
			}

			TempStore.clear();
			//LOG("Thread ID : %d, ObjectPool Test Complete #%d\n", this_thread::get_id(), TestCount);
			std::cout << "ObjectPool Test Complete #" << TestCount << endl;
			++TestCount;

			// For Another Thread - Context Switch
			std::this_thread::sleep_for(0s);
		}
	};

	Enqueue_Agent_Task(function, 1);
	Enqueue_Agent_Task(function, 2);
	Enqueue_Agent_Task(function, 3);
	Enqueue_Agent_Task(function, 4);
	Enqueue_Agent_Task(function, 5);
	Enqueue_Agent_Task(function, 6);
	Enqueue_Agent_Task(function, 7);
	Enqueue_Agent_Task(function, 8);
	Enqueue_Agent_Task(function, 9);



	CObjectPoolTLS<Test> TestPool(1000, 10000);
	while (1)
	{
		std::this_thread::sleep_for(1s);
		std::ostringstream stringStream;
		stringStream << "Object Pool AllocCount : ";
		stringStream << TestPool.GetAllocCount();

		SetConsoleTitle(stringStream.str().c_str());
	}

	return 0;
}