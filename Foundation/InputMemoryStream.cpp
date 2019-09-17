#include "pch.h"
#include "InputMemoryStream.h"

using namespace Util;

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

	_mutex.lock();

	memcpy(outData, _buffer + _head, inByteCount);
	_head += inByteCount;

	_mutex.unlock();
}


void InputMemoryStream::Serialize(void* outData, uint32_t inByteCount)
{
	Read(outData, inByteCount);
}




