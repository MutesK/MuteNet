//
// Created by junmkim on 2020-11-13.
//

#include "Common.h"
#include "TypeDefine.hpp"
#include "SocketDescriptor.h"

namespace EventLoop
{
    ISocketDescriptor::ISocketDescriptor ( RawIOContextImplPtr const &Ptr, socket_t Socket )
    :IEventBaseComponent(Ptr), _socket(Socket)
    {

    }
    
    
    void ISocketDescriptor::SetCallback ( CallbackPtr ReadCallback, CallbackPtr WriteCallback, ExceptCallackPtr ExceptionCallback,
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
}