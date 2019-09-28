#include "CircularTest.h"

using namespace Util;
int main()
{
	CircularTest Test;

	while (true)
	{

		Test.SendPacket();
		Test.RecvProcess();
	}
}