#include "../ConcurrentProgrammingModules/CommonHeader.h"
#include "HeapBlock.h"
#include <chrono>
#include <thread>

using namespace std::chrono_literals;
HeapBlock buffer;

int main()
{
	std::thread test = std::thread([&]()
		{
			while (true)
			{
				char str[6];

				for (int i = 0; i < 1000; i++)
				{
					_itoa_s(i, str, 6, 10);

					if (buffer.PutData(str, sizeof(int)) == 0)
						throw;
				}			

				for (auto i = 0; i < 1000; i++)
				{
					if (buffer.GetData(str, sizeof(int)) != sizeof(int))
						throw;

					int test = atoi(str);

					if (test != i)
						throw;
				}
			}
		});


	std::thread bufferchecker = std::thread([&]()
		{
			auto start = std::chrono::high_resolution_clock::now();

			while (true)
			{
				std::cout << "\x1B[2J\x1B[H"; // Clear Console
				auto end = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> elapsed = end - start;

				std::cout << "Working Time : " << elapsed.count() << "s\n";
				std::cout << "Buffer Use Count : " << buffer.GetUseSize() << std::endl;
				std::cout << "Buffer Free Size : " << buffer.GetFreeSize() << std::endl;

				std::this_thread::sleep_for(1s);
			}
		});


	test.join();
	bufferchecker.join();
}