#include "OutputMemoryStream.h"

OutputMemoryStream::OutputMemoryStream()
	:_buffer(nullptr), _head(0), _capacity(0)
{
}

OutputMemoryStream::~OutputMemoryStream()
{
	std::free(_buffer);
}

void OutputMemoryStream::Write(const void* inData, const uint32_t inByteCount)
{
	const uint32_t resultPosition = _head + inByteCount;
	
	if (resultPosition > _capacity)
		ReallocBuffer(std::max(_capacity * 2, resultPosition));

	std::memcpy(_buffer + _head, inData, inByteCount);
	_head += inByteCount;
}


void OutputMemoryStream::Serialize(void* inData, uint32_t inByteCount)
{
	Write(inData, inByteCount);
}

void OutputMemoryStream::ReallocBuffer(const uint32_t inNewLength)
{
	_buffer = static_cast<char*>(std::realloc(_buffer, inNewLength));

	if (_buffer == nullptr)
		throw;

	_capacity = inNewLength;
}

