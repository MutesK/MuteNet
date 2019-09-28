#include "pch.h"
#include "HeapBlock.h"

using namespace Util;

TL::ObjectPool<HeapBlock> HeapBlock::BlockPool;

std::shared_ptr<HeapBlock> Util::HeapBlock::make_shared()
{
	return BlockPool.make_shared();
}

Util::HeapBlock::~HeapBlock()
{
	std::free(_buffer);
}

void HeapBlock::Write(const void* inData, const uint32_t bytesize)
{
	const uint32_t resultPosition = _tail + bytesize;

	if (resultPosition > _capacity)
		ReallocBuffer(std::max(_capacity * 2, resultPosition));

	std::memcpy(_buffer + _tail, inData, bytesize);
	_tail += bytesize;
}

void HeapBlock::Read(void* outData, const uint32_t bytesize)
{
	if (outData == nullptr)
		throw;

	if (_head + bytesize > _capacity)
		throw;

	memcpy(outData, _buffer + _head, bytesize);
	_head += bytesize;
}

void HeapBlock::ReallocBuffer(int bufferSize)
{
	_buffer = static_cast<char*>(std::realloc(_buffer, bufferSize));

	if (_buffer == nullptr)
		throw;

	_capacity = bufferSize;
}
