#include "HeapBlock.h"
#include <cstring>

HeapBlock::HeapBlock()
{
	_buffer = new char[5000];
	memset(_buffer, 0, 5000);
	_capacity = 5000;
}

HeapBlock::~HeapBlock()
{
	Clear();
	delete[] _buffer;
}

uint32_t HeapBlock::PutData(const char* Data, uint32_t Size)
{
	if (Size > GetFreeSize())
		return 0;

	if (Size <= 0)
		return 0;

	if(_front <= _rear)
	{
		const auto notBrokenWriteSize = _capacity - _rear;

		if(notBrokenWriteSize >= Size)
		{
			memcpy(_buffer + _rear, Data, Size);
			_rear += Size;
		}
		else
		{
			memcpy(_buffer + _rear, Data, notBrokenWriteSize);
			memcpy(_buffer, (Data + notBrokenWriteSize), Size - notBrokenWriteSize);
			_rear = Size - notBrokenWriteSize;
		}
	}
	else
	{
		memcpy(_buffer + _rear, Data, Size);
		_rear += Size;
	}

	return Size;
}

uint32_t HeapBlock::GetData(char* Data, uint32_t Size)
{
	if (Size <= 0)
		return 0;

	if (Size > GetUseSize())
		return 0;

	if(_front <= _rear)
	{
		memcpy(Data, _buffer + _front, Size);
		_front += Size;
	}
	else
	{
		const auto notBrokenReadSize = _capacity - _front;

		if(notBrokenReadSize >= Size)
		{
			memcpy(Data, _buffer + _front, Size);
			_front += Size;
		}
		else
		{
			memcpy(Data, _buffer + _front, notBrokenReadSize);
			memcpy(Data + notBrokenReadSize, _buffer, Size - notBrokenReadSize);
			_front = Size - notBrokenReadSize;
		}
	}

	return Size;
}

uint32_t HeapBlock::Peek(char* Data, uint32_t Size)
{
	if (Size <= 0)
		return 0;

	if (Size > GetUseSize())
		return 0;

	if (_front <= _rear)
	{
		memcpy(Data, _buffer + _front, Size);
	}
	else
	{
		const auto notBrokenReadSize = _capacity - _front;

		if (notBrokenReadSize >= Size)
		{
			memcpy(Data, _buffer + _front, Size);
		}
		else
		{
			memcpy(Data, _buffer + _front, notBrokenReadSize);
			memcpy(Data + notBrokenReadSize, _buffer, Size - notBrokenReadSize);
		}
	}

	return Size;
}

void HeapBlock::MoveInputPosition(uint32_t Size)
{
	if (Size > GetFreeSize())
		return;

	if (Size <= 0)
		return;

	if (_front <= _rear)
	{
		const auto notBrokenWriteSize = _capacity - _rear;

		if (notBrokenWriteSize >= Size)
			_rear += Size;
		else
			_rear = Size - notBrokenWriteSize;

	}
	else
		_rear += Size;
}

void HeapBlock::MoveOutputPosition(uint32_t Size)
{
	if (Size <= 0)
		return;

	if (Size > GetUseSize())
		return;

	if (_front <= _rear)
	{
		_front += Size;
	}
	else
	{
		const auto notBrokenReadSize = _capacity - _front;

		if (notBrokenReadSize >= Size)
		{
			_front += Size;
		}
		else
		{
			_front = Size - notBrokenReadSize;
		}
	}
}
