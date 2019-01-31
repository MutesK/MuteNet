#include <iostream>
#include "ObjectPoolTLS.h"
#include <thread>
#include <vector>

using namespace std;


enum ValidCode
{
	FOR_VAILD = 0x9876
};
class Lab
{
public:

	Lab()
	{
	}
	~Lab()
	{
	}
	union vaild
	{
		unsigned short makecount;
		unsigned short vaildcode;
		int	  fullvaild;
	};

	vaild vaildNum;
};

CObjectPoolTLS<Lab> _testTls;
std::vector<std::thread> _testThreadPool;

int main()
{
	for(int i=0; i<5; i++)
		_testThreadPool.push_back(std::thread([&]()
	{
		while (1)
		{
			list<std::shared_ptr<Lab>> list;

			for (int i = 0; i < 100000; i++)
				list.push_back(_testTls.get_shared());

			std::this_thread::sleep_for(1s);

			for (int i = 0; i < 100000; i++)
				list.pop_back();
		}
	}));

	for (int i = 0; i < 5; i++)
	{
		_testThreadPool[i].joinable();
	}

	while (1)
	{
		std::cout << "Object Pool TLS 부하 테스트\n";
		std::cout << "Alloced Chunk Block Size : " << _testTls.GetChunkSize() << std::endl;
		std::cout << "Total Alloc Data Size : " << _testTls.GetAllocCount() << std::endl;
		std::this_thread::sleep_for(1s);
		system("cls");
	}

	return 0;
}