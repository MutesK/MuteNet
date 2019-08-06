#include "InputByteStream.h"

namespace Util
{
	InputByteStream::InputByteStream(char* buffer, uint32_t inByteCount)
		:InputMemoryStream(buffer, inByteCount)
	{
	}

	void InputByteStream::Read(void* outData, uint32_t inByteCount)
	{
		if (inByteCount > GetUseDataSize())
			throw;

		if (inByteCount <= 0)
			throw;

		if (_head <= _tail)
		{
			memcpy(outData, _buffer + _head, inByteCount);
			_head += inByteCount;
			return;
		}

		// Paste Data
		const int length = _capacity - _head;

		if(length >= inByteCount)
		{
			memcpy(outData, _buffer + _head, inByteCount);
			_head += inByteCount;
			return;
		}

		memcpy(outData, _buffer + _head, length);
		memcpy(reinterpret_cast<char *>(outData) + length, _buffer, inByteCount - length);
		
	}

}
