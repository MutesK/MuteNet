// Base Develop.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

/*
	This Project is Only Test And Experience C++ MultiThread Programming Skils

*/
#include "stdafx.h"
#include "Thread.h"

#include "ObjectPoolTLS.hpp" 
#include "ConsolePrinter.h"
#include <thread>

struct Test
{
	int a; 

	Test()
	{

	}
};

int main()
{


	auto function = []()
	{
		CObjectPoolTLS<Test> TestPool(1000, 10000, true);
		vector<Test *>	   TempStore;
		uint64_t			TestCount = 0;

		while (1)
		{
			for (int i = 0; i < 50000; i++)
			{
				auto variable = TestPool.Alloc();
				if (variable == nullptr)
				{
					*((int *)0) = 1;
				}

				variable->a = i;
				TempStore.push_back(variable);
			}

			for (auto ptr : TempStore)
			{
				if (TestPool.Free(ptr) == false)
				{
					*((int *)0) = 1;
				}
			}

			TempStore.clear();
			//LOG("ObjectPool Test", 1, "Thread ID : %d, ObjectPool Test Complete #%d\n", this_thread::get_id(), TestCount);
			std::cout << "ObjectPool Test Complete #" << TestCount << endl;
			++TestCount;

			// For Another Thread - Context Switch
			std::this_thread::sleep_for(0s);
		}
	};

	thread th1(function);
	thread th2(function);
	thread th3(function);
	thread th4(function);
	thread th5(function);


	CObjectPoolTLS<Test> TestPool(1000, 10000, true);
	while(1)
	{ 
		std::this_thread::sleep_for(1s);
		std::ostringstream stringStream;
		stringStream << "Object Pool AllocCount : ";
		stringStream << TestPool.GetAllocCount();

		SetConsoleTitle(stringStream.str().c_str());
	}

    return 0;
}

