#include "PathImpl.h"
#include "Exception.h"

std::string PathImpl::current()
{
	std::string result;
	DWORD len = GetCurrentDirectory(0, nullptr);

	if (len > 0)
	{
		char *buffer = new char[len + 1];
		DWORD ret = GetCurrentDirectory(len, buffer);

		if (ret > 0 && ret <= len)
		{
			result = buffer;

			if (result[result.size() - 1] != '\\')
				result.append("\\");

			return result;
		}
	}

	throw SystemException("Invalid Current Path");
}

std::string PathImpl::self()
{
	char buffer[50000];

	DWORD ret = GetModuleFileName(nullptr, buffer, 50000);
	DWORD err = GetLastError();

	if (ret > 0)
	{
		if (err == ERROR_INSUFFICIENT_BUFFER)
			throw SystemException("Buffer is too small to get Path");

		return buffer;
	}

	throw SystemException("Cannot get Executable File");
}

std::string PathImpl::expand(const std::string& path)
{
	char buffer[30000];

	DWORD ret = ExpandEnvironmentStrings(path.c_str(), buffer, 30000);
	if (ret > 0 && ret < 30000 - 1)
	{
		buffer[ret + 1] = '\0';
		return buffer;
	}

	return path;
}

void PathImpl::listDriver(std::vector<std::string>& OUT roots)
{
	roots.clear();

	const int buffersize = 128;
	char buffer[buffersize];

	DWORD ret = GetLogicalDriveStrings(buffersize - 1, buffer);
	
	char *pStart = buffer;
	char *pEnd = buffer + (ret > buffersize ? buffersize : ret);

	while (pStart < pEnd)
	{
		std::string driver;
		while (pStart < pEnd && *pStart)
		{
			driver += *pStart++;
		}

		roots.push_back(driver);
		++pStart;
	}
}
