#pragma once

/*
	ȯ�溯��, OS �̸�, ���μ��� ī��Ʈ �� �����´�.
*/
#include "foundation.h"

using IPAddress = size_t[6];

class EnvrironmentImpl
{
public:
	static std::string getEnv(const std::string& IN name);
	static bool hasEnv(const std::string& IN name);
	static void setEnv(const std::string& IN name, const std::string& IN value);
	// Deprecated function
	// ���� ��κ� �ü���� Windows 7, Windows Server 2008, Windows Vista,
	// Windows Server 2003, Windows XP, or Windows 2000 �̹Ƿ�, �з��� �� �Լ��� �ǹ̰� ����.
	// osDisplayName���� ����Ұ�.
	static std::string osName();
	static std::string osDisplayName();
	static std::string osVersion();
	static std::string osArchitecture();
	static std::string computerName();
	static void ipAddress(IPAddress& OUT ipAddress);
};

