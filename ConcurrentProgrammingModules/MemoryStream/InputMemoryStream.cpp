#include "InputMemoryStream.h"

InputMemoryStream::InputMemoryStream(char* buffer, uint32_t inByteCount)
	:_buffer(buffer), _capacity(inByteCount)
{
}

InputMemoryStream::~InputMemoryStream()
{
	std::free(_buffer);
}

// Heap Corruption À¯ÀÇ
void InputMemoryStream::Read(void* outData, uint32_t inByteCount)
{
	if (outData == nullptr)
		throw;

	if (_head + inByteCount >= _capacity)
		throw;

	memcpy(outData, _buffer + _head, inByteCount);
	_head += inByteCount;
}


void InputMemoryStream::Serialize(void* inData, uint32_t inByteCount)
{
	Read(inData, inByteCount);
}




