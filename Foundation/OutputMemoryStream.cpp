#include "pch.h"
#include "OutputMemoryStream.h"

using namespace Util;

TL::ObjectPool<OutputMemoryStream> OutputMemoryStream::_objectPool;

OutputMemoryStream::OutputMemoryStream()
	:_buffer(nullptr), _head(0), _capacity(0)
{
	_objectPool.setDynamicAlloc(true);
}

OutputMemoryStream::~OutputMemoryStream()
{
	std::free(_buffer);
}

std::shared_ptr<OutputMemoryStream> Util::OutputMemoryStream::GenerateStream()
{
	return _objectPool.make_shared();
}

void OutputMemoryStream::Write(const void* inData, const uint32_t inByteCount)
{
	_mutex.lock();

	const uint32_t resultPosition = _head + inByteCount;
	
	if (resultPosition > _capacity)
		ReallocBuffer(std::max(_capacity * 2, resultPosition));

	std::memcpy(_buffer + _head, inData, inByteCount);
	_head += inByteCount;

	_mutex.unlock();

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

