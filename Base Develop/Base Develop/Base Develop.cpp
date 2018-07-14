// Base Develop.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

/*
	This Project is Only Test And Experience C++ MultiThread Programming Skils

*/
#include "stdafx.h"
#include "Thread.h"
#include "Task.h"

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
	dolamda dolamda = [testvariable = 1, dotest = 2]()
	{
		while (1)
		{
			std::cout << "This Function is Running in Task and Main Thread" << std::endl;
			std::cout << "Argument Lambda Function Test\n";

			std::cout << testvariable << " " << dotest << std::endl;
		}
	};

	Task task(dolamda, __FUNCTION__, __FILE__, __LINE__);

	task.Do();



    return 0;
}

