#include "pch.h"
#include "MemoryStream.h"

namespace Util
{
	MemoryStream::MemoryStream()
	{
		_buffer = new Byte[_capacity];
	}

	MemoryStream::~MemoryStream()
	{
		delete[] _buffer;
	}
}
