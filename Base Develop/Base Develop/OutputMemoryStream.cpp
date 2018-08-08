#include "MemoryStream.h"
#include "OutputMemoryStream.h"


OutputMemoryStream::OutputMemoryStream()
	:_buffer(nullptr), _head(0), _capacity(0)
{
	ReallocBuffer(50000);
}


OutputMemoryStream::~OutputMemoryStream()
{
	
}

std::shared_ptr<char>& OutputMemoryStream::GetBufferPtr()
{
	return _buffer;
}

uint32_t OutputMemoryStream::GetLength() const
{
	return _head;
}

void OutputMemoryStream::ReallocBuffer(uint32_t inNewLength)
{
	_buffer = std::shared_ptr<char>(new char[inNewLength], std::default_delete<char>());

	if (_buffer == nullptr)
	{
		*((int *)0) = 1; // Crash
	}

	_capacity = inNewLength;
}

void OutputMemoryStream::Write(const void* inData, size_t inByteCount)
{
	uint32_t resultHead = _head + static_cast<uint32_t> (inByteCount);

	if (resultHead > _capacity)
		ReallocBuffer(std::max(_capacity * 2, resultHead));

	std::memcpy(_buffer.get() + _head, inData, inByteCount);
	_head += inByteCount;
}

void OutputMemoryStream::Serialize(void *ioData, uint32_t Bytecount)
{
	Write(ioData, Bytecount);
}

bool OutputMemoryStream::IsInput() const
{
	return false;
}