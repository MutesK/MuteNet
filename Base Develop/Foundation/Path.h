#pragma once

#include "foundation.h"

class Path
{
public:
	using Directions = std::vector<std::string>;

	Path();
	~Path();

	void assign(const std::string& path);
	void parse(const std::string& path);
	void makeDirectory();

private:
	Directions	_dir;
	bool		_isAbsolutedir;
};

