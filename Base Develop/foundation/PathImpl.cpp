#include "Path.h"
#include "Exception.h"

std::string PathImpl::currentImpl()
{
	std::string result;

	DWORD len = GetCurrentDirectory(0, nullptr);
	if (len > 0)
	{
		// 차후 buffer 자료구조 만들어야된다.	
		char* buffer = new char[len];

		DWORD ret = GetCurrentDirectory(len, buffer);
		if (ret > 0 && ret <= len)
		{
			result = buffer;
			
			if (result[result.size() - 1] != '\\')
				result.append("\\");

			return result;
		}
	}

	throw Exception("Cannot get Current Directory");
}

std::string PathImpl::expandImpl(const std::string& path)
{
	std::string _path;

	char* buffer = new char[MAX_LEN];
	DWORD n = ExpandEnvironmentStrings(_path.c_str(), buffer, MAX_LEN);
	if (n > 0 && n <= MAX_LEN)
	{
		buffer[n + 1] = '\0';
		std::string result = buffer;
		return result;
	}
	else
		return path;

}

void PathImpl::rootImpl(std::vector<std::string>& OUT roots)
{
	roots.clear();

	const int buffersize = 128;
	char buffer[buffersize];

	DWORD n = GetLogicalDriveStrings(buffersize - 1, buffer);
	char* pStart = buffer;
	char *pEnd = buffer + (n > buffersize ? buffersize : n);

	while (pStart < pEnd)
	{
		std::string element;
		while (pStart < pEnd && *pStart)
			element += *pStart++;

		roots.emplace_back(element);
		++pStart;
	}

}