#pragma once

#include "../ConcurrentProgrammingModules/CommonHeader.h"

class MemoryStream abstract
{
	virtual void Serialize(void* inData, uint32_t inByteCount) = 0;

	template <typename Type>
	void Serialize(Type& Data);

	virtual bool IsInput() = 0;
};

template <typename Type>
void MemoryStream::Serialize(Type& Data)
{
	static_assert(
		std::is_arithmetic_v<Type> || std::is_enum_v<Type>,
		"Generic Write Only Supports Primitive Data Types");

	Serialize(&Data, sizeof(Type));
}

