#include "EnvrironmentImpl.h"
#include "Exception.h"


std::string EnvrironmentImpl::getEnv(const std::string& IN name)
{
	DWORD len = GetEnvironmentVariable(name.c_str(), nullptr, 0);
	if (len == 0)
		throw NotFoundException(name);

	char buffer[20000];
	GetEnvironmentVariable(name.c_str(), buffer, 20000);
	return buffer;
}

bool EnvrironmentImpl::hasEnv(const std::string& IN name)
{
	DWORD len = GetEnvironmentVariable(name.c_str(), nullptr, 0);

	return len > 0;
}

void EnvrironmentImpl::setEnv(const std::string& IN name, const std::string& IN value)
{
	if (SetEnvironmentVariable(name.c_str(), value.c_str()) == 0)
	{
		throw SystemException("");
	}
}

std::string EnvrironmentImpl::osName()
{
	OSVERSIONINFO info;
	info.dwOSVersionInfoSize = sizeof(info);

	if (GetVersionEx(&info) == 0)
		throw SystemException("Can't get OS Version Information");

	switch (info.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_NT:
		return "Windows NT";
	default:
		return "Unknown";
	}
}

std::string EnvrironmentImpl::osDisplayName()
{
	OSVERSIONINFOEX info;
	info.dwOSVersionInfoSize = sizeof(info);

	if(GetVersionEx((OSVERSIONINFO *)&info) == 0)
		throw SystemException("Can't get OS Version Information");

	switch (info.dwMajorVersion)
	{
	case 10:
		switch (info.dwMinorVersion)
		{
		case 0:
			return info.wProductType == VER_NT_WORKSTATION ? "Windows 10" : "Windows Server 2016";
		}
		break;
	case 6:
		switch (info.dwMinorVersion)
		{
		case 0:
			return info.wProductType == VER_NT_WORKSTATION ? "Windows Vist" : "Windows Server 2008";
		case 1:
			return info.wProductType == VER_NT_WORKSTATION ? "Windows 7" : "Windows Server 2008 R2";
		case 2:
			return info.wProductType == VER_NT_WORKSTATION ? "Windows 8" : "Windows Server 2012";
		case 3:
			return info.wProductType == VER_NT_WORKSTATION ? "Windows 8.1" : "Windows Server 2012 R2";
		default:
			return "Unknown";
		}
		break;
	case 5:
		switch()
	}

}