#pragma once

#include "SafeSharedLock.h"

namespace Util
{
	class CircularBuffer final
	{
	private:
		static const size_t BLANK_BUFFER = 1;

		std::atomic_bool    _IsSending = false;
		SafeSharedLock		_Lock;
		char*				_Buffer;
		int					_Front;
		int					_Rear;

		int					_BufferSize;
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
		return _Buffer;
	}
	inline char* CircularBuffer::GetReadBufferPtr(void)
	{
		return _Buffer + _Front;
	}
	inline char* CircularBuffer::GetWriteBufferPtr(void)
	{
		return _Buffer + _Rear;
	}
	inline SafeSharedLock& CircularBuffer::GetSharedLock()
	{
		return _Lock;
	}
}