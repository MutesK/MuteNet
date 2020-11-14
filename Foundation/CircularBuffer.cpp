#include "FoundationCommon.h"
#include "CircularBuffer.h"

namespace Util
{
	const size_t CircularBuffer::BLANK_BUFFER;
	
	
	CircularBuffer::CircularBuffer ( )
			: _Front ( 0 ), _Rear ( 0 ), _BufferSize ( BUFFER_LENGTH )
	{
		_Buffer = new char[BUFFER_LENGTH];
		memset ( _Buffer, 0, BUFFER_LENGTH );
		
	}
	
	CircularBuffer::CircularBuffer ( int iSize )
			: _BufferSize ( iSize ), _Front ( 0 ), _Rear ( 0 )
	{
		_Buffer = new char[iSize];
		memset ( _Buffer, 0, iSize );
	}
	
	CircularBuffer::~CircularBuffer ( )
	{
		delete[] _Buffer;
	}
	
	CircularBuffer &CircularBuffer::operator= ( const CircularBuffer &RightBuffer )
	{
		return *this;
	}
	
	int CircularBuffer::GetBufferSize ( void )
	{
		return _BufferSize - BLANK_BUFFER;
	}
	
	int CircularBuffer::GetUseSize ( void )
	{
		if ( _Rear >= _Front )
		{
			return _Rear - _Front;
		} else
		{
			return _BufferSize - _Front + _Rear;
		}
	}
	
	int CircularBuffer::GetFreeSize ( void )
	{
		return _BufferSize - GetUseSize ( );
	}
	
	int CircularBuffer::GetWriteBufferAndLengths ( char **firstbuf, uint32_t &firstlength, char **secondbuf,
	                                               uint32_t &secondlength )
	{
		int bufcount = 1;
		*firstbuf = GetWriteBufferPtr ( );
		firstlength = GetNotBrokenPutSize ( );
		
		if ( firstlength < static_cast<uint32_t>(GetFreeSize ( )))
		{
			bufcount++;
			*secondbuf = GetBufferPtr ( );
			secondlength = GetFreeSize ( ) - firstlength;
		}
		
		return bufcount;
	}
	
	int CircularBuffer::GetReadBufferAndLengths ( char **firstbuf, uint32_t &firstlength, char **secondbuf,
	                                              uint32_t &secondlength )
	{
		int bufcount = 1;
		*firstbuf = GetReadBufferPtr ( );
		firstlength = GetNotBrokenGetSize ( );
		
		if ( firstlength < static_cast<uint32_t>(GetUseSize ( )))
		{
			bufcount++;
			*secondbuf = GetBufferPtr ( );
			secondlength = GetUseSize ( ) - firstlength;
		}
		
		return bufcount;
		
	}
	
	int CircularBuffer::GetNotBrokenGetSize ( void )
	{
		if ( _Front <= _Rear )
		{
			return _Rear - _Front;
		} else
		{
			return _BufferSize - _Front;
		}
	}
	
	int CircularBuffer::GetNotBrokenPutSize ( void )
	{
		if ( _Rear < _Front )
		{
			return _Front - _Rear - BLANK_BUFFER;
		} else
		{
			if ( _Front < BLANK_BUFFER )
			{
				return _BufferSize - _Rear - BLANK_BUFFER + _Front;
			} else
			{
				return _BufferSize - _Rear;
			}
		}
	}
	
	int CircularBuffer::PutData ( void *chpData, int iSize )
	{
		int inputSize = 0;
		
		if ( iSize > GetFreeSize ( ))
		{
			throw;
		}
		
		if ( iSize <= 0 )
		{
			throw;
		}
		
		if ( _Front <= _Rear )
		{
			int WriteSize = _BufferSize - _Rear;
			
			if ( WriteSize >= iSize )
			{
				memcpy ( _Buffer + _Rear, chpData, iSize );
				_Rear += iSize;
			} else
			{
				memcpy ( _Buffer + _Rear, chpData, WriteSize );
				memcpy ( _Buffer, reinterpret_cast<char *>(chpData) + WriteSize,
				         static_cast<size_t>(iSize) - WriteSize );
				_Rear = iSize - WriteSize;
			}
			
		} else
		{
			memcpy ( _Buffer + _Rear, chpData, iSize );
			_Rear += iSize;
		}
		
		return iSize;
	}
	
	int CircularBuffer::GetData ( void *chpDest, int iSize )
	{
		if ( iSize > GetUseSize ( ))
		{
			throw;
		}
		
		if ( iSize <= 0 )
		{
			throw;
		}
		
		if ( _Front <= _Rear )
		{
			memcpy ( chpDest, _Buffer + _Front, iSize );
			_Front += iSize;
		} else
		{
			int ReadSize = _BufferSize - _Front;
			
			if ( ReadSize >= iSize )
			{
				memcpy ( chpDest, _Buffer + _Front, iSize );
				_Front += iSize;
			} else
			{
				memcpy ( chpDest, _Buffer + _Front, ReadSize );
				memcpy ( reinterpret_cast<char *>(chpDest) + ReadSize, _Buffer, static_cast<size_t>(iSize) - ReadSize );
				_Front = iSize - ReadSize;
			}
		}
		
		return iSize;
	}
	
	int CircularBuffer::Peek ( void *chpDest, int iSize )
	{
		if ( iSize > GetUseSize ( ))
		{
			return 0;
		}
		
		if ( iSize <= 0 )
		{
			return 0;
		}
		
		if ( _Front <= _Rear )
		{
			memcpy ( chpDest, _Buffer + _Front, iSize );
		} else
		{
			int ReadSize = _BufferSize - _Front;
			
			if ( ReadSize >= iSize )
			{
				memcpy ( chpDest, _Buffer + _Front, iSize );
			} else
			{
				memcpy ( chpDest, _Buffer + _Front, ReadSize );
				memcpy ( reinterpret_cast<char *>(chpDest) + ReadSize, _Buffer, static_cast<size_t>(iSize) - ReadSize );
			}
		}
		
		return iSize;
	}
	
	void CircularBuffer::MoveReadPostion ( int iSize )
	{
		if ( iSize > GetUseSize ( ))
		{
			throw;
		}
		
		if ( iSize <= 0 )
		{
			throw;
		}
		
		if ( _Front <= _Rear )
		{
			_Front += iSize;
		} else
		{
			int ReadSize = _BufferSize - _Front;
			
			if ( ReadSize >= iSize )
			{
				_Front += iSize;
			} else
			{
				_Front = iSize - ReadSize;
			}
		}
		if ( _Front == _BufferSize )
		{
			_Front = 0;
		}
	}
	
	void CircularBuffer::MoveWritePos ( int iSize )
	{
		int inputSize = 0;
		
		if ( iSize > GetFreeSize ( ))
		{
			throw;
		}
		
		if ( iSize <= 0 )
		{
			throw;
		}
		
		if ( _Front <= _Rear )
		{
			int WriteSize = _BufferSize - _Rear;
			
			if ( WriteSize >= iSize )
			{
				_Rear += iSize;
			} else
			{
				_Rear = iSize - WriteSize;
			}
			
		} else
		{
			_Rear += iSize;
		}
	}
	
	
}