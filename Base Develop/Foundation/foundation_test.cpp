#include "foundation.h"
#include "PathImpl.h"

using namespace std;
int main()
{
	std::vector<std::string> drivers;

	std::cout << "Get Driver List" << std::endl;
	PathImpl::listDriver(drivers);
	for (auto driver : drivers)
	{
		std::cout << driver << std::endl;
	}

	std::cout << "Get Current" << std::endl;
	cout << PathImpl::current() << endl;

	cout << "Get Self" << endl;
	cout << PathImpl::self() << endl;

	cout << "Get Expand" << endl;
	string expand = "%APPDATA%";
	cout << "Parameter : " << expand << " result : " << PathImpl::expand(expand) << endl;

	return 0;
}