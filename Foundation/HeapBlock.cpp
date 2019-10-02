#include "pch.h"
#include "HeapBlock.h"

using namespace Util;


const size_t HeapBlock::BUFFER_LENGTH;

std::shared_ptr<HeapBlock> Util::HeapBlock::make_shared()
{
	return std::make_shared<HeapBlock>();
}

Util::HeapBlock::HeapBlock()
{
	_buffer = new char[_capacity];
}

Util::HeapBlock::~HeapBlock()
{
	delete[] _buffer;
}

void HeapBlock::Write(const void* inData, const size_t bytesize)
{
	const uint64_t resultPosition = _tail + bytesize;

	if (resultPosition > _capacity)
		throw;

	std::memcpy(_buffer + _tail, inData, bytesize);
	_tail += bytesize;
}

void HeapBlock::Read(void* outData, const size_t bytesize)
{
	if (outData == nullptr)
		throw;

	if (_head + bytesize > _capacity)
		throw;

	memcpy(outData, _buffer + _head, bytesize);
	_head += bytesize;
}

char* HeapBlock::GetBufferPtr()
{
	return _buffer;
}

void HeapBlock::MoveWritePosition(const size_t length)
{
	_tail += length;
}
