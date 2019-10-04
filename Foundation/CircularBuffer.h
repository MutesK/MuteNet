#pragma once

#include "SafeSharedLock.h"

namespace Util
{
	class CircularBuffer final
	{
	private:
		static const size_t BUFFER_SIZE = 10000;
		static const size_t BLANK_BUFFER = 1;

		std::atomic_bool    _isSending = false;
		SafeSharedLock		_lock;
		char*				_buffer;
		int					_front;
		int					_rear;

		int					_bufferSize;
	public:
		CircularBuffer();
		CircularBuffer(int);
		~CircularBuffer();

		CircularBuffer& operator=(const CircularBuffer& RightBuffer);

		int	GetBufferSize(void);

		int	GetUseSize(void);
		int	GetFreeSize(void);
	
		int GetWriteBufferAndLengths(char** firstbuf, uint32_t& firstlength,
			char** secondbuf, uint32_t& secondlength);
		int GetReadBufferAndLengths(char** firstbuf, uint32_t& firstlength,
			char** secondbuf, uint32_t& secondlength);

		int	PutData(void* chpData, int iSize);
		int	GetData(void* chpDest, int iSize);
		int	Peek(void* chpDest, int iSize);

		void MoveReadPostion(int iSize);
		void MoveWritePos(int iSize);
	
		char* GetBufferPtr(void);
		char* GetReadBufferPtr(void);
		char* GetWriteBufferPtr(void);

		SafeSharedLock& GetSharedLock();

		GET_SET_ATTRIBUTE(bool, isSending);
	private:

		int	GetNotBrokenGetSize(void);
		int	GetNotBrokenPutSize(void);
	};

	inline char* CircularBuffer::GetBufferPtr(void)
	{
		return _buffer;
	}
	inline char* CircularBuffer::GetReadBufferPtr(void)
	{
		return _buffer + _front;
	}
	inline char* CircularBuffer::GetWriteBufferPtr(void)
	{
		return _buffer + _rear;
	}
	inline SafeSharedLock& CircularBuffer::GetSharedLock()
	{
		return _lock;
	}
}