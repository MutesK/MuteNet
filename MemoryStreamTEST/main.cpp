#include "StreamTest.h"

using namespace Util;
int main()
{
	while (true)
	{
		StreamTest Test;

		Test.EnqueueDataStream();
		Test.TestGetBufferElements();
	}
}