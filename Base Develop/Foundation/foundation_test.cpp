#include "foundation.h"
#include "foundation.hpp"

int main()
{
	try
	{
		throw CException("test");
	}
	catch (CMemoryException& e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (CFileException& e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (CException& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}