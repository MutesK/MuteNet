#pragma once

#include <functional>

using IOCompletion = std::function<void(void*)>;

class IOService abstract
{
protected:
	IOCompletion _OnIOCompletion;
public:
	virtual void SetOnIOCompletion(const IOCompletion& CallBack) = 0;
};

