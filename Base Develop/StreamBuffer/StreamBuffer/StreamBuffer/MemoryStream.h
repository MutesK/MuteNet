#pragma once

#include <iostream>
#include <algorithm>
#include <memory>

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

