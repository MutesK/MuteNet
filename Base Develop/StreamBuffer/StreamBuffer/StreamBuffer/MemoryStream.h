#pragma once


#include <iostream>
#include <algorithm>
#include <memory>
/*
	Output 스트림 버퍼와, Input 스트림 버퍼를 업캐스팅하여 사용함.
*/
class MemoryStream
{
public:
	virtual void Serialize(void* ioData, uint32_t ByteCount) = 0;
	virtual bool IsInput() const = 0;

	template<typename type>
	void Serialize(type& ioData)
	{
		static_assert(std::is_arithmetic<type>::value || std::is_enum<type>::isvalue, "Generic Write Only Supports Primitive Data Type");

		Serialize(&ioData, sizeof(ioData));
	}
};

