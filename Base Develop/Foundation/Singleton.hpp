
#pragma once 

#include "Singleton.h"

template<class Type>
std::shared_ptr<Type> Singleton<Type>::_instance = nullptr;


template<class Type>
std::once_flag Singleton<Type>::_flag;