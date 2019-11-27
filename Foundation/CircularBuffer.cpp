#include "pch.h"
#include "CircularBuffer.h"

namespace Util
{
	const size_t CircularBuffer::BUFFER_SIZE;
	const size_t CircularBuffer::BLANK_BUFFER;


	CircularBuffer::CircularBuffer()
		:_front(0), _rear(0), _bufferSize(BUFFER_SIZE)
	{
		_buffer = new char[BUFFER_SIZE];
		memset(_buffer, 0, BUFFER_SIZE);

	}

	CircularBuffer::CircularBuffer(int iSize)
		:_bufferSize(iSize), _front(0), _rear(0)
	{
		_buffer = new char[iSize];
		memset(_buffer, 0, iSize);
	}

	CircularBuffer::~CircularBuffer()
	{
		delete[] _buffer;
	}

	CircularBuffer& CircularBuffer::operator=(const CircularBuffer& RightBuffer)
	{
		return *this;
	}

	int	CircularBuffer::GetBufferSize(void)
	{
		return _bufferSize - BLANK_BUFFER;
	}

	int	CircularBuffer::GetUseSize(void)
	{
		if (_rear >= _front)
			return _rear - _front;
		else
			return _bufferSize - _front + _rear;
	}

	int	CircularBuffer::GetFreeSize(void)
	{
		return _bufferSize - GetUseSize();
	}

	int CircularBuffer::GetWriteBufferAndLengths(char** firstbuf, uint32_t& firstlength, char** secondbuf, uint32_t& secondlength)
	{
		int bufcount = 1;
		*firstbuf = GetWriteBufferPtr();
		firstlength = GetNotBrokenPutSize();

		if (firstlength < static_cast<uint32_t>(GetFreeSize()))
		{
			bufcount++;
			*secondbuf = GetBufferPtr();
			secondlength = GetFreeSize() - firstlength;
		}

		return bufcount;
	}

	int CircularBuffer::GetReadBufferAndLengths(char** firstbuf, uint32_t& firstlength, char** secondbuf, uint32_t& secondlength)
	{
		int bufcount = 1;
		*firstbuf = GetReadBufferPtr();
		firstlength = GetNotBrokenGetSize();

		if (firstlength < static_cast<uint32_t>(GetUseSize()))
		{
			bufcount++;
			*secondbuf = GetBufferPtr();
			secondlength = GetUseSize() - firstlength;
		}

		return bufcount;

	}

	int	CircularBuffer::GetNotBrokenGetSize(void)
	{
		if (_front <= _rear)
		{
			return _rear - _front;
		}
		else
			return _bufferSize - _front;
	}
	int	CircularBuffer::GetNotBrokenPutSize(void)
	{
		if (_rear < _front)
		{
			return _front - _rear - BLANK_BUFFER;
		}
		else
		{
			if (_front < BLANK_BUFFER)
				return _bufferSize - _rear - BLANK_BUFFER + _front;
			else
				return _bufferSize - _rear;
		}
	}

	int	CircularBuffer::PutData(void* chpData, int iSize)
	{
		int inputSize = 0;

		if (iSize > GetFreeSize())
			throw;

		if (iSize <= 0)
			throw;

		if (_front <= _rear)
		{
			int WriteSize = _bufferSize - _rear;

			if (WriteSize >= iSize)
			{
				memcpy(_buffer + _rear, chpData, iSize);
				_rear += iSize;
			}
			else
			{
				memcpy(_buffer + _rear, chpData, WriteSize);
				memcpy(_buffer, reinterpret_cast<char *>(chpData) + WriteSize, static_cast<size_t>(iSize) - WriteSize);
				_rear = iSize - WriteSize;
			}

		}
		else
		{
			memcpy(_buffer + _rear, chpData, iSize);
			_rear += iSize;
		}

		if (_rear == _bufferSize)
			_rear = 0;

		return iSize;
	}

	int	CircularBuffer::GetData(void* chpDest, int iSize)
	{
		if (iSize > GetUseSize())
			throw;

		if (iSize <= 0)
			throw;

		if (_front <= _rear)
		{
			memcpy(chpDest, _buffer + _front, iSize);
			_front += iSize;
		}
		else
		{
			int ReadSize = _bufferSize - _front;

			if (ReadSize >= iSize)
			{
				memcpy(chpDest, _buffer + _front, iSize);
				_front += iSize;
			}
			else
			{
				memcpy(chpDest, _buffer + _front, ReadSize);
				memcpy(reinterpret_cast<char*>(chpDest) + ReadSize, _buffer, static_cast<size_t>(iSize) - ReadSize);
				_front = iSize - ReadSize;
			}
		}

		if (_rear == _front)
			_rear = _front = 0;

		return iSize;
	}

	int	CircularBuffer::Peek(void* chpDest, int iSize)
	{
		if (iSize > GetUseSize())
			return 0;

		if (iSize <= 0)
			return 0;

		if (_front <= _rear)
		{
			memcpy(chpDest, _buffer + _front, iSize);
		}
		else
		{
			int ReadSize = _bufferSize - _front;

			if (ReadSize >= iSize)
			{
				memcpy(chpDest, _buffer + _front, iSize);
			}
			else
			{
				memcpy(chpDest, _buffer + _front, ReadSize);
				memcpy(reinterpret_cast<char*>(chpDest) + ReadSize, _buffer, static_cast<size_t>(iSize) - ReadSize);
			}
		}

		return iSize;
	}

	void CircularBuffer::MoveReadPostion(int iSize)
	{
		if (iSize > GetUseSize())
			throw;

		if (iSize <= 0)
			throw;

		if (_front <= _rear)
		{
			_front += iSize;
		}
		else
		{
			int ReadSize = _bufferSize - _front;

			if (ReadSize >= iSize)
			{
				_front += iSize;
			}
			else
			{
				_front = iSize - ReadSize;
			}
		}
		if (_front == _bufferSize)
			_front = 0;
	}

	void CircularBuffer::MoveWritePos(int iSize)
	{
		int inputSize = 0;

		if (iSize > GetFreeSize())
			throw;

		if (iSize <= 0)
			throw;

		if (_front <= _rear)
		{
			int WriteSize = _bufferSize - _rear;

			if (WriteSize >= iSize)
			{
				_rear += iSize;
			}
			else
			{
				_rear = iSize - WriteSize;
			}

		}
		else
		{
			_rear += iSize;
		}

		if (_rear == _bufferSize)
			_rear = 0;
	}

}