#include "../ConcurrentProgrammingModules/CommonHeader.h"
#include "CircularBuffer.h"

namespace Util
{
	CircularBuffer::CircularBuffer()
	{
		_buffer = new char[BUFFER_SIZE];
	}

	CircularBuffer::~CircularBuffer()
	{
		delete[] _buffer;
	}

	void CircularBuffer::GetBufferPtr(void* firstBuffer, size_t& firstLength, void* secondBuffer, size_t& secondLength)
	{
	}

	void CircularBuffer::MoveReadPostion(const size_t& position)
	{
		const auto moveposition = std::min(position, GetUsedSize());

		std::lock_guard<std::recursive_mutex> lock(_mutex);

		if (isEmpty())
		{
			return;
		}

		if (_head <= _tail)
		{
			_head += moveposition;
			return;
		}

		const auto brokenmoveposition = BUFFER_SIZE - _head;
		if (brokenmoveposition >= moveposition)
		{
			_head += moveposition;
			return;
		}

		_head = moveposition - brokenmoveposition;
	}

	size_t CircularBuffer::Peek(void* Data, const size_t size)
	{
		if (nullptr == Data)
		{
			return false;
		}

		std::lock_guard<std::recursive_mutex> lock(_mutex);

		const auto copybytes = std::min(size, GetUsedSize());

		if (isEmpty())
		{
			return false;
		}

		if (_head <= _tail)
		{
			memcpy(Data, _buffer + _head, copybytes);
			return copybytes;
		}

		const auto brokencopybytes = BUFFER_SIZE - _head;
		if (brokencopybytes >= copybytes)
		{
			memcpy(Data, _buffer + _head, copybytes);
			return copybytes;
		}

		memcpy(Data, _buffer + _head, brokencopybytes);
		memcpy(static_cast<char*>(Data) + brokencopybytes, _buffer, copybytes - brokencopybytes);

		return copybytes;
	}

	size_t CircularBuffer::GetData(void* outData, const size_t size)
	{
		const auto bytes = Peek(outData, size);
		MoveReadPostion(bytes);

		return bytes;
	}

	void CircularBuffer::MoveWritePostion(const size_t& position)
	{
		if (position <= 0)
			return;

		std::lock_guard<std::recursive_mutex> lock(_mutex);

		if (position > GetFreeSize())
		{
			return;
		}

		if (_head <= _tail)
		{
			const auto brokenputbytes = BUFFER_SIZE - _tail;

			if (brokenputbytes >= position)
			{
				_tail += position;
				return;
			}

			 _tail = position - brokenputbytes;
			 return;
		}

		_tail += position;
		return;
	}

	size_t CircularBuffer::PutData(void* inData, const size_t size)
	{
		if (nullptr == inData)
		{
			return false;
		}

		if (size <= 0)
			return false;

		std::lock_guard<std::recursive_mutex> lock(_mutex);

		if (size > GetFreeSize())
		{
			return false;
		}
		
		if (_head <= _tail)
		{
			const auto brokenputbytes = BUFFER_SIZE - _tail;

			if (brokenputbytes >= size)
			{
				memcpy(_buffer + _tail, inData, size);
				_tail += size;
				return size;
			}

			memcpy(_buffer + _tail, inData, brokenputbytes);
			memcpy(_buffer, static_cast<char*>(inData) + brokenputbytes, size - brokenputbytes);
			_tail = size - brokenputbytes;
			return size;
		}

		memcpy(_buffer + _tail, inData, size);
		_tail += size;
		return size;
		
	}

}