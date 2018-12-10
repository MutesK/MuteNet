#pragma once

#include "foundation.h"

class DirectoryIteratorImpl
{
public:
	DirectoryIteratorImpl(const std::string& path);
	~DirectoryIteratorImpl();

private:
	HANDLE			_handle;
	WIN32_FIND_DATA _findData;
	std::string		_current;
	int				_rc;
};

