#pragma once

/*
	환경변수, OS 이름, 프로세서 카운트 등 가져온다.
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
	// 현재 대부분 운영체제는 Windows 7, Windows Server 2008, Windows Vista,
	// Windows Server 2003, Windows XP, or Windows 2000 이므로, 분류의 이 함수의 의미가 없음.
	// osDisplayName으로 사용할것.
	static std::string osName();
	static std::string osDisplayName();
	static std::string osVersion();
	static std::string osArchitecture();
	static std::string computerName();
	static void ipAddress(IPAddress& OUT ipAddress);
};

