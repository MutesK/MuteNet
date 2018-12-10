#include "foundation.h"
#include "Path.h"
using namespace std;
int main()
{
	std::vector <std::string> vect;
	PathImpl::rootImpl(vect);

	for (auto iter : vect)
	{
		cout << iter << endl;
	}

	return 0;
}