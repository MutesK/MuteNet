#include "Exception.h"



Exception::Exception(std::exception& src)
{
	_message = src.what();
}


Exception::~Exception()
{
}

Exception::Exception(const char* const msg)
{
	_message = msg;
}


const char* Exception::what() const 
{
	return _message.c_str();
}
