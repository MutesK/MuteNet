//
// Created by junmkim on 2020-11-13.
//

#include "Common.h"
#include "TypeDefine.hpp"
#include "SocketDescriptor.h"

#include <OutputMemoryStream.h>
#include <InputMemoryStream.h>

namespace EventLoop
{
	struct Buffer
	{
		char* BufferPtr;
		uint32_t length;
	};
	
	ISocketDescriptor::ISocketDescriptor ( RawIOContextImplPtr const &Ptr, socket_t Socket )
			: IEventBaseComponent ( Ptr ), _socket ( Socket ), Util::AtomicCounter()
	{
	
	}
	
	ISocketDescriptor::~ISocketDescriptor ( )
	{
		_ExceptCallback(this, 0, _Key);
	}
	
	void ISocketDescriptor::SetCallback ( CallbackPtr ReadCallback, CallbackPtr WriteCallback,
	                                      ExceptCallackPtr ExceptionCallback,
	                                      void *Key )
	{
		_ReadCallback = ReadCallback;
		_WriteCallback = WriteCallback;
		_ExceptCallback = ExceptionCallback;
		_Key = Key;
	}
	
	socket_t ISocketDescriptor::GetFD ( ) const
	{
		return _socket;
	}
	
	bool ISocketDescriptor::_Read ( )
	{
		Util::OutputMemoryStream StreamBuffer;
		auto& InputMemoryStream = reinterpret_cast<Util::InputMemoryStream &>(StreamBuffer);
		
		
		int ret = recv(_socket, const_cast<char *>(InputMemoryStream.GetBufferPtr()),
		               InputMemoryStream.GetRemainingDataSize(), 0);
		
		if(ret == SOCKET_ERROR)
		{
			delete this;
			return false;
		}
		
		if(ret == 0)
		{
			return true;
		}
		
		StreamBuffer.MoveWritePosition(ret);
		
		{
			std::unique_lock<std::shared_mutex> lock ( _ReadBuffer._mutex );
			_ReadBuffer.PutData ( const_cast<char *>(StreamBuffer.GetBufferPtr ( )), StreamBuffer.GetLength ( ));
		}
		
		return true;
	}
	
	bool ISocketDescriptor::_Send ( )
	{
		Util::OutputMemoryStream StreamBuffer;
		auto& InputMemoryStream = reinterpret_cast<Util::InputMemoryStream &>(StreamBuffer);
		
		Buffer Buffer[2];
		
		{
			std::unique_lock<std::shared_mutex> lock ( _WriteBuffer._mutex );
			
			auto SendSize = _WriteBuffer.GetUseSize ( );
			
			if ( SendSize <= 0 )
				return true;
			
			_WriteBuffer.GetReadBufferAndLengths ( &Buffer[ 0 ].BufferPtr, Buffer[ 0 ].length,
			                                       &Buffer[ 1 ].BufferPtr, Buffer[ 1 ].length );
			
			StreamBuffer.Serialize ( Buffer[ 0 ].BufferPtr, Buffer[ 0 ].length );
			if ( Buffer[ 0 ].length > SendSize )
			{
				StreamBuffer.Serialize ( Buffer[ 1 ].BufferPtr, Buffer[ 1 ].length );
			}
		}
		
		int ret = send(_socket, InputMemoryStream.GetBufferPtr(), InputMemoryStream.GetRemainingDataSize(), 0);
		
		if(ret == SOCKET_ERROR)
		{
			delete this;
			return false;
		}
		
		{
			std::unique_lock<std::shared_mutex> lock ( _WriteBuffer._mutex );
			
			_WriteBuffer.MoveReadPostion(ret);
		}
		
		return true;
		
	}
	
	Util::InputMemoryStream ISocketDescriptor::GetReadBuffer ( ) const
	{
		Util::InputMemoryStream Stream;
		
		Buffer Buffer[2];
		
		_ReadBuffer.GetReadBufferAndLengths ( &Buffer[ 0 ].BufferPtr, Buffer[ 0 ].length,
		                                       &Buffer[ 1 ].BufferPtr, Buffer[ 1 ].length );
		
		
		reinterpret_cast<Util::OutputMemoryStream &>(Stream).Serialize(Buffer[ 0 ].BufferPtr, Buffer[ 0 ].length);
		
		if(Buffer[0].length < _ReadBuffer.GetUseSize())
		{
			reinterpret_cast<Util::OutputMemoryStream &>(Stream).Serialize(Buffer[ 1 ].BufferPtr, Buffer[ 1 ].length);
		}
		
		return Stream;
	}

    bool ISocketDescriptor::IsVaildSocket()
    {
        return false;
    }


}