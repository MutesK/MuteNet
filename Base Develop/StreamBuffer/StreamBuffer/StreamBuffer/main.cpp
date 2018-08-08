#include "MemoryStream.h"
#include "OutputMemoryStream.h"
#include "InputMemoryStream.h"
#include <memory>
#include <limits.h>
#include <Windows.h>
using namespace std;

int main()
{
	uint64_t testcase = 1;

 	while (true)
	{
		cout << "TestCount : " << testcase << endl;

		testcase++;

		OutputMemoryStream SendStream;

		// 1 ~ INT MAX까지 넣고 빼면서 확인
		for (int i = 1; i < 12500; i++)
		{
			SendStream << i;
		}

		InputMemoryStream RecvStream(SendStream.GetBufferPtr(), SendStream.GetLength());

		for (int i = 1; i < 12500; i++)
		{
			int outdata;
			RecvStream >> outdata;

			if (i != outdata)
			{
				*((int *)0) = 1; // Crash
			}
		}

		Sleep(0);

	}


	return 0;
}