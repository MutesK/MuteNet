#include "stdafx.h"
#include "MemoryStream.h"
#include "OutputMemoryStream.h"
#include "InputMemoryStream.h"



InputMemoryStream::InputMemoryStream(std::shared_ptr<char>& buffer, uint32_t inByteCount)
	:_capacity(inByteCount), _head(0), _buffer(buffer)
{
}

InputMemoryStream::InputMemoryStream(OutputMemoryStream& outputStream)
	:_capacity(outputStream.GetLength()), _head(0), _buffer(outputStream.GetBufferPtr())
{

}

InputMemoryStream::~InputMemoryStream()
{
}

void InputMemoryStream::Read(void *pOutData, uint32_t ByteCount)
{
	if (_head + ByteCount > _capacity)
	{
		*((int *)0) = 1; // Crash
	}

	memcpy(pOutData, _buffer.get() + _head, ByteCount);
	_head += ByteCount;
}

void InputMemoryStream::Serialize(void *ioData, uint32_t Bytecount)
{
	Read(ioData, Bytecount);
}

bool InputMemoryStream::IsInput() const 
{
	return true;
}