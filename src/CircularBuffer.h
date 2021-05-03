#pragma once

namespace Util
{
	class CircularBuffer
	{
	private:
		static const size_t BUFFER_LENGTH = 20000;
		static const size_t BLANK_BUFFER = 1;


		char* _Buffer;
		int _Front;
		int _Rear;

		int _BufferSize;
	public:
		CircularBuffer();

		CircularBuffer(int);

		~CircularBuffer();

		CircularBuffer& operator= (const CircularBuffer& RightBuffer);

		int GetBufferSize(void) const;

		int GetUseSize(void) const;

		int GetFreeSize(void) const;

		int GetWriteBufferAndLengths(char** firstbuf, uint32_t& firstlength,
			char** secondbuf, uint32_t& secondlength) const;

		int GetReadBufferAndLengths(char** firstbuf, uint32_t& firstlength,
			char** secondbuf, uint32_t& secondlength) const;

		int PutData(void* chpData, int iSize);

		int GetData(void* chpDest, int iSize);

		int Peek(void* chpDest, int iSize);

		void MoveReadPostion(int iSize);

		void MoveWritePos(int iSize);

		char* GetBufferPtr(void) const;

		char* GetReadBufferPtr(void) const;

		char* GetWriteBufferPtr(void) const;

	private:
		int GetNotBrokenGetSize(void) const;

		int GetNotBrokenPutSize(void) const;
	};

	inline char* CircularBuffer::GetBufferPtr(void) const
	{
		return _Buffer;
	}

	inline char* CircularBuffer::GetReadBufferPtr(void) const
	{
		return _Buffer + _Front;
	}

	inline char* CircularBuffer::GetWriteBufferPtr(void) const
	{
		return _Buffer + _Rear;
	}
}