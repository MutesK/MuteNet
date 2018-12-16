#pragma once

#include "foundation.h"

class PathImpl
{
public:
	static std::string current();
	static std::string self();
	static std::string expand();
	static void listDriver(std::vector<std::string>& roots);
};

