#include "EnvironmentImpl.h"
#include "Exception.h"
#include <IPTypes.h>
#include <iphlpapi.h>

#pragma comment(lib, "IPHLPAPI.lib")

std::string EnvrironmentImpl::getEnv(const std::string& IN name)
{
	DWORD len = GetEnvironmentVariable(name.c_str(), nullptr, 0);
	if (len == 0)
		throw NotFoundException(name.c_str());

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
	OSVERSIONINFO info{0};
	info.dwOSVersionInfoSize = sizeof(info);

	// if (GetVersionEx(&info) == 0)
		// throw SystemException("Can't get OS Version Information");

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
	OSVERSIONINFOEX info { 0 };
	info.dwOSVersionInfoSize = sizeof(info);

	//if(GetVersionEx((OSVERSIONINFO *)&info) == 0)
//		throw SystemException("Can't get OS Version Information");

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
		switch (info.dwMinorVersion)
		{
		case 0:
			return "Windows 2000";
		case 1:
			return "Windows XP";
		case 2:
			return "Windows Server2003/Windows Server 2003 R2";
		default:
			return "Unknown";
		}
		break;
	default:
		return "Unknown";
	}

	return "Unknown";
}

std::string EnvrironmentImpl::osVersion()
{
	OSVERSIONINFOW info{0};
	info.dwOSVersionInfoSize = sizeof(info);

//	if(GetVersionExW(&info) == 0)
	//	throw SystemException("Can't get OS Version Information");

	std::ostringstream stream;
	stream << info.dwMajorVersion << "." << info.dwMajorVersion << " (Build "
		<< (info.dwBuildNumber & 0xFFFF);
	
	return stream.str();
}
// 차후 작업
std::string EnvrironmentImpl::osArchitecture()
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);

	switch (info.wProcessorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_INTEL:
		return "IA32";
	case PROCESSOR_ARCHITECTURE_IA64:
		return "IA64";
	default:
		return "Unknown";
	}
}

std::string EnvrironmentImpl::computerName()
{
	char name[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD size = MAX_COMPUTERNAME_LENGTH + 1;

	if (GetComputerName(name, &size) == 0)
		throw SystemException("Cannot get computer name");

	return name;
}

void EnvrironmentImpl::ipAddress(IPAddress& OUT ipAddress)
{

}