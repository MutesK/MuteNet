#include <iostream>
#include <thread>
#include <array>
#include "..//ConcurrentProgrammingModules/CommonHeader.h"
#include "CircularBuffer.h"
#include "../ConcurrentProgrammingModules/ObjectPool.h"

using namespace Util;
using namespace std;

void bufferTest()
{
	CircularBuffer buffer;
	std::cout << "Single Thread CircularBuffer I/O Test \n";

	size_t testcase = 1;
	auto startTime = std::chrono::system_clock::now();

	while (true)
	{
		system("cls");
		std::cout << "===================== CIRCULAR BUFFER TEST =====================\n";
		std::cout << "Test Cast : " << testcase << std::endl;
		std::cout << "Work Time(sec) : "
			<< std::chrono::duration<double>(std::chrono::system_clock::now() - startTime).count() << std::endl;
		std::cout << "\n\n";

		for (auto i = 0; i < 5000; i++)
		{
			if (buffer.PutData(&i, sizeof(int)) != sizeof(int))
			{
				throw std::exception("Buffer Input Error");
			}
		}

		for (auto i = 0; i < 5000; i++)
		{
			int test = 0;

			if (buffer.GetData(&test, sizeof(int)) != sizeof(int))
			{
				throw std::exception("Buffer Output Error");
			}


			if (test != i)
			{
				throw std::exception("Buffer Output Circulartion is Error");
			}

		}

		testcase++;
		std::this_thread::sleep_for(1s);
	}
}
int main()
{
	struct TestStruct
	{
		int vaild;
	};

	auto startTime = std::chrono::system_clock::now();
	TL::ObjectPool<TestStruct> structPool;

	auto test = [&structPool]()
	{
		while (true)
		{
			std::list<TestStruct *> list;

			for (auto i = 0; i < 10000; i++)
			{
				auto ptr = structPool.Alloc();
				list.push_back(ptr);
			}

			for (auto ptr : list)
			{
				structPool.Free(ptr);
			}
		}
	};

	std::array<std::thread, 5> threadpool;
	for (int i = 0; i < 5; i++)
	{
		threadpool[i] = std::thread(test);
	}

	while (true)
	{
		system("cls");
		std::cout << "===================== OBJECT POOL TEST =====================\n";
		std::cout << "Work Time(sec) : "
			<< std::chrono::duration<double>(std::chrono::system_clock::now() - startTime).count() << std::endl;
		std::cout << "Object Pool Use Count : " << structPool.UseCount() << std::endl;
		std::cout << "\n\n";

		std::this_thread::sleep_for(1s);
	}
}