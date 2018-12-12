#pragma once

#include "foundation.h"

// test 

template<class Type>
class Singleton
{
public:
	static std::shared_ptr<Type>& GetInstance()
	{
		std::call_once(_flag, [&]() {
			_instance = std::make_shared<Type>();
		});

		return _instance;
	}
private:
	static std::shared_ptr<Type> _instance;
	static std::once_flag		 _flag;
};
