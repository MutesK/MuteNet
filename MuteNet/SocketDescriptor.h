//
// Created by junmkim on 2020-11-13.
//

#ifndef MUTENET_SOCKETDESCRIPTOR_H
#define MUTENET_SOCKETDESCRIPTOR_H

#include "EventBaseComponent.hpp"

namespace EventLoop
{
    class SocketDescriptor;
    using CallbackPtr = void (*)(SocketDescriptor*, void* Self);
    using ExceptCallackPtr = void (*)(SocketDescriptor*, uint16_t What, void* Self);

    class SocketDescriptor : public IEventBaseComponent
    {
        socket_t  _socket;

        CallbackPtr _ReadCallback;
        CallbackPtr _WriteCallback;
        ExceptCallackPtr _ExceptCallback;
        void*            _Key;

        friend class IOContextImpl;
        friend class SelectIOContext;

        SocketDescriptor(const RawIOContextImplPtr &Ptr, socket_t Socket);

    public:
        void Read(void* data, size_t length);
        void Write(void* data, size_t length);

        void Enable(uint16_t Flag);
        void Disable(uint16_t Flag);

        void Shutdown();

        void SetCallback(CallbackPtr ReadCallback, CallbackPtr WriteCallback,
                         ExceptCallackPtr ExceptionCallback, void* Key);

        socket_t GetFD() const;
    };
}


#endif //MUTENET_SOCKETDESCRIPTOR_H
