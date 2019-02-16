// FoundationTest.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <list>
#include <iostream>
#include "../Foundation/ObjectPoolTLS.h"
#include <thread>

using namespace std;

class Obj
{
public:
	Obj(int a, int b, int c, int d)
	{
		std::cout << a << b << c << d;
	}
	Obj(int a, int b, int c, int d, int e, int f)
	{

	}
	Obj(int a, int b)
	{

	}
	Obj(int a)
	{}
	Obj()
	{

	}
	Obj(Obj&& p)
	{

	}
};
CObjectPoolTLS<Obj> _testTls;
std::vector<std::thread> _testThreadPool;

int main()
{
	for (int i = 0; i < 5; i++)
		_testThreadPool.push_back(std::thread([&]()
	{
		while (1)
		{
			list<std::shared_ptr<Obj>> list;

			for (int i = 0; i < 100000; i++)
				list.push_back(_testTls.get_shared());

			std::this_thread::sleep_for(1s);

			for (int i = 0; i < 100000; i++)
				list.pop_back();

			std::this_thread::sleep_for(1s);
		}

	}));

	for (int i = 0; i < 5; i++)
	{
		_testThreadPool[i].joinable();
	}

	while (1)
	{
		std::cout << "Object Pool TLS \n";
		std::cout << "Alloced Chunk Block Size : " << _testTls.GetChunkSize() << std::endl;
		std::cout << "Total Alloc Data Size : " << _testTls.GetAllocCount() << std::endl;
		std::this_thread::sleep_for(0.2s);
		system("cls");
	}

	return 0;
}