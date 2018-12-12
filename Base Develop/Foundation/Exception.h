#pragma once

#include "foundation.h"

// Avoid two Symbol
namespace
{

	//class Excpetion : public std::exception
	//{
	//public:														
	//	Excpetion(const char* const msg)
	//	{
	//		_message = msg;
	//	}
	//	Excpetion(std::exception& src)
	//	{
	//		_message = src.what();
	//	}
	//	const char* what() const
	//	{
	//		return _message.c_str();
	//	}
	//protected:													
	//	std::string _message;									
	//};

#define DECLARE_EXCEPTION(NAME)							\
	class NAME : public std::exception	\
	{	\
	public:	\
		NAME(const char* const msg)	\
		{	\
			_message = "[";	\
			_message += #NAME;	\
			_message += "] : ";	\
			_message += msg;	\
		}	\
		NAME(std::exception& src)	\
		{	\
			_message = "[";	\
			_message += #NAME;	\
			_message += "] : ";	\
			_message += src.what();	\
		}	\
		const char* what() const	\
		{	\
			return _message.c_str();	\
		}	\
	protected:	\
		std::string _message;	\
	};	\



	DECLARE_EXCEPTION(SystemException);
	DECLARE_EXCEPTION(CMemoryException);
	DECLARE_EXCEPTION(CFileException);
	DECLARE_EXCEPTION(CException);

};