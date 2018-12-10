#pragma once

#include "foundation.h"

enum
{
	MAX_LEN = 32000
};

class PathImpl
{
public:
	static std::string currentImpl();
	static std::string expandImpl(const std::string& path);
	static void rootImpl(std::vector<std::string>& OUT roots);

};

