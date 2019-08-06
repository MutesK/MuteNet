#include "OutputByteStream.h"

namespace Util
{
	OutputByteStream::OutputByteStream()
		:OutputMemoryStream()
	{
	}

	uint32_t OutputByteStream::GetLength() const
	{
		if (_tail >= _head)
			return _tail - _head;

		return _capacity - _head + _tail;
	}

	void OutputByteStream::Write(const void* inData, uint32_t inByteCount)
	{
		if (!inData)
			return;
		
		if (inByteCount > GetFreeSize())
			return;

		if (inByteCount <= 0)
			return;

		if(_head <= _tail)
		{
			const size_t length = _capacity - _tail;

			if(length >= inByteCount)
			{
				memcpy(_buffer + _tail, inData, length);
				_tail += length;
				return;
			}

			memcpy(_buffer + _tail, inData, length);
			memcpy(_buffer, reinterpret_cast<const char *>(inData) + length, inByteCount - length);
		}
	}

	int OutputByteStream::GetFreeSize() const
	{
		return _capacity - GetLength();
	}
}
