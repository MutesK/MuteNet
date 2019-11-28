#include "pch.h"
#include "InputMemoryStream.h"
using namespace Util;

// Heap Corruption À¯ÀÇ
void InputMemoryStream::Read(void* outData, uint32_t inByteCount)
{
	if (outData == nullptr)
		throw;

	if (_head + inByteCount > static_cast<const uint64_t>(_capacity))
		throw;

	memcpy(outData, _buffer + _head, inByteCount);
	_head += inByteCount;
}

void InputMemoryStream::Serialize(void* outData, uint32_t inByteCount)
{
	uint32_t Size = 0;

	Read(&Size, sizeof(uint32_t));

	if (Size > inByteCount)
	{
		outData = nullptr;
		return;
	}

	Read(outData, Size);
}




