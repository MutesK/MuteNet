#include "pch.h"
#include "HeapBlock.h"

using namespace Util;

TL::ObjectPool<HeapBlock, true, true> HeapBlock::BlockPool;
const size_t HeapBlock::BUFFER_LENGTH;

const auto Deletor = [&](HeapBlock* pBlock)
{
	HeapBlock::BlockPool.Free(pBlock);
};

std::shared_ptr<HeapBlock> Util::HeapBlock::Alloc()
{
	const auto Ptr = BlockPool.Alloc();

	return std::shared_ptr<HeapBlock>(Ptr, Deletor);
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

	if (resultPosition > static_cast<const uint64_t>(_capacity))
		throw;

	std::memcpy(_buffer + _tail, inData, bytesize);
	_tail += bytesize;
}

void HeapBlock::Read(void* outData, const size_t bytesize)
{
	if (outData == nullptr)
		throw;

	if (_head + bytesize > static_cast<const uint64_t>(_capacity))
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

int64_t HeapBlock::GetRemainingDataSize() const
{
	return _tail - _head;
}