#include "foundation.h"
#include "Singleton.hpp"
#include "MiniDump.h"

static MiniDump dump;
int main()
{

	// Crash Test;
	int *p = nullptr;
	(*p) = 1;

	return 0;
}