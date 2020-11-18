//
// Created by Mute on 2020-11-15.
//

#ifndef MUTENET_UNIXLIKEDESCRIPTOR_HPP
#define MUTENET_UNIXLIKEDESCRIPTOR_HPP

#include "Descriptor.h"

#ifdef POSIX_PLATFORM

namespace EventLoop
{
    class UnixLikeDescriptor : public IDescriptor
    {
        friend class IUnixLikeIOContextImpl;

    public:
        UnixLikeDescriptor(const RawIOContextImplPtr &Ptr, socket_t Socket);;

        virtual ~UnixLikeDescriptor();

        virtual void Write(void *data, size_t length) override;

        virtual void Enable() override;

        virtual void Disable(uint16_t Flag) override;

    private:
        virtual void Read() override;

    protected:
        virtual bool _Read() override;

        virtual bool _Write() override;
    };
}

#endif

#endif //MUTENET_UNIXLIKEDESCRIPTOR_HPP
