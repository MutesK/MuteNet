#include "RingBuffer.h"
#include <random>
#include <thread>
#include <chrono>
int main()
{
	RingBuffer buffer;

	auto test_unit = [&buffer]() {
		std::random_device rn;
		std::mt19937_64 rnd(rn());

		while (1)
		{
			std::uniform_int_distribution<int> range(0, 5000);
			auto count = range(rnd);

			for (int i = 0; i < count - 1; i++)
			{
				buffer << i;
			}
			
			for (int i = 0; i < count - 1; i++)
			{
				int temp;
				buffer >> temp;

				if (temp != i)
				{
					int *p = 0;
					*p = 1;
				}
					
			}

			if (buffer.GetUseSize() != 0)
			{
				int *p = 0;
				*p = 1;
			}

		}
	};

	std::thread th1(test_unit);
	th1.joinable();

	while (1)
	{
		std::cout << "Ring Buffer (Byte) 부하 테스트\n";
		std::cout << "Use Byte : " << buffer.GetUseSize() << std::endl;
		std::cout << "Free Byte : " << buffer.GetFreeSize() << std::endl;

		Sleep(1000);
		system("cls");
	}

}