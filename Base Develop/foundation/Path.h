#pragma once

#include "foundation.h"

class Path
{
public:
	using Stringlist = std::vector<std::string>;

	Path();
	Path(const std::string& path);


	~Path();
};

