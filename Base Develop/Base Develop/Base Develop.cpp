// Base Develop.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

/*
	This Project is Only Test And Experience C++ MultiThread Programming Skils

*/
#include "stdafx.h"
#include "Thread.h"
#include "Task.h"
#include "MemoryPool.h"
#include <thread>

class Test : public Thread
{
public:
	Test()
	{
		_threadstoprequest = false;

		SetThreadName(std::string("Test Thread"));
	}
	~Test()
	{

	}

private:
	virtual void DoWork()
	{
		int TestVariable = 0;
		while (!_threadstoprequest)
		{
			std::cout << "Thread ID : " << GetCurrentThreadId() << "Independent Variable :" << TestVariable << std::endl;
			TestVariable++;

			Sleep(1000);
		}
	}
	virtual void EmitWakeupSignal()
	{
		_threadstoprequest = true;
	}

private:
	bool _threadstoprequest;
};


int main()
{
	CMemoryPool<__int64> ObjectPoolTest;  // Memory Pool Lock Free 낙관적 락 테스트

	std::function<void()> alloc = [&ObjectPoolTest]()
	{
		__int64 tester = 0;

		while (1)
		{
			auto value = ObjectPoolTest.Alloc();

			*value = tester;

			printf("Address Check : 0x%p\n", value);

			ObjectPoolTest.Free(value);
			tester++;

			Sleep(10);
		}
	};

	std::thread th1(alloc);
	std::thread th2(alloc);
	std::thread th3(alloc);
	std::thread th4(alloc);
	std::thread th5(alloc);
	std::thread th6(alloc);
	std::thread th7(alloc);
	std::thread th8(alloc);


	th1.joinable();
	th2.joinable();
	th3.joinable();
	th4.joinable();
	th5.joinable();
	th6.joinable();
	th7.joinable();
	th8.joinable();


	while (1);

    return 0;
}

