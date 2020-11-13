//
// Created by junmkim on 2020-11-13.
//

#include "Common.h"
#include "TypeDefine.hpp"
#include "SocketDescriptor.h"

namespace EventLoop
{
    SocketDescriptor::SocketDescriptor ( RawIOContextImplPtr const &Ptr, socket_t Socket )
    :IEventBaseComponent(Ptr), _socket(Socket)
    {

    }

    void SocketDescriptor::Read ( void *data, size_t length )
    {

    }

    void SocketDescriptor::Write ( void *data, size_t length )
    {

    }

    void SocketDescriptor::Enable ( uint16_t Flag )
    {

    }

    void SocketDescriptor::Disable ( uint16_t Flag )
    {

    }

    void SocketDescriptor::SetCallback ( CallbackPtr ReadCallback, CallbackPtr WriteCallback, ExceptCallackPtr ExceptionCallback,
                                         void *Key )
    {
        _ReadCallback = ReadCallback;
        _WriteCallback = WriteCallback;
        _ExceptCallback = ExceptionCallback;
        _Key = Key;
    }

    socket_t SocketDescriptor::GetFD ( ) const
    {
        return _socket;
    }

}