
#pragma once

#include "foundation.h"

class Exception : public std::exception
{
public:
	Exception(const char* const);
	Exception(std::exception& src);
	virtual ~Exception();

	const char* what() const;
protected:
	std::string _message;
};



	