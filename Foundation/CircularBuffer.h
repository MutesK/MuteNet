#pragma once

namespace Util
{
	class CircularBuffer
	{
	private:
		static const size_t BUFFER_LENGTH = 20000;
		static const size_t BLANK_BUFFER = 1;
		
		
		char *_Buffer;
		int _Front;
		int _Rear;
		
		int _BufferSize;
	public:
		CircularBuffer ( );
		
		CircularBuffer ( int );
		
		~CircularBuffer ( );
		
		CircularBuffer &operator= ( const CircularBuffer &RightBuffer );
		
		int GetBufferSize ( void );
		
		int GetUseSize ( void );
		
		int GetFreeSize ( void );
		
		int GetWriteBufferAndLengths ( char **firstbuf, uint32_t &firstlength,
		                               char **secondbuf, uint32_t &secondlength );
		
		int GetReadBufferAndLengths ( char **firstbuf, uint32_t &firstlength,
		                              char **secondbuf, uint32_t &secondlength );
		
		int PutData ( void *chpData, int iSize );
		
		int GetData ( void *chpDest, int iSize );
		
		int Peek ( void *chpDest, int iSize );
		
		void MoveReadPostion ( int iSize );
		
		void MoveWritePos ( int iSize );
		
		char *GetBufferPtr ( void );
		
		char *GetReadBufferPtr ( void );
		
		char *GetWriteBufferPtr ( void );
	
	private:
		int GetNotBrokenGetSize ( void );
		
		int GetNotBrokenPutSize ( void );
	};
	
	inline char *CircularBuffer::GetBufferPtr ( void )
	{
		return _Buffer;
	}
	
	inline char *CircularBuffer::GetReadBufferPtr ( void )
	{
		return _Buffer + _Front;
	}
	
	inline char *CircularBuffer::GetWriteBufferPtr ( void )
	{
		return _Buffer + _Rear;
	}
}