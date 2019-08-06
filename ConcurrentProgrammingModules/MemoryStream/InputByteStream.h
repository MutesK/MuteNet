#pragma once

#include "InputMemoryStream.h"

/*
 *	Like a Ring Buffer.
 *	
 *	
 */
namespace Util
{
	class InputByteStream : InputMemoryStream
	{
	private:
		uint32_t _tail = 0;
	public:
		InputByteStream(char* buffer, uint32_t inByteCount);
		virtual ~InputByteStream() = default;

		uint32_t GetRemainingDataSize() const override;
		void Read(void* outData, uint32_t inByteCount) override;

	private:
		int GetUseDataSize() const;
	};

	inline 	uint32_t InputByteStream::GetRemainingDataSize() const
	{
		return _capacity - GetUseDataSize();
	}

	inline int InputByteStream::GetUseDataSize() const
	{
		if (_tail >= _head)
			return _tail - _head;

		return _capacity - _head + _tail;
	}

}

