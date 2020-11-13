//
// Created by junmkim on 2020-11-13.
//

#ifndef MUTENET_SELECTIOCONTEXT_HPP
#define MUTENET_SELECTIOCONTEXT_HPP

#include "EventBaseComponent.hpp"
#include "IoContextImpl.hpp"
#include "SocketDescriptor.h"


namespace EventLoop
{
    class SelectIOContext : public IOContextImpl,
                            public Util::Runnable
    {
        struct ContextContainer
        {
            std::mutex          _SwapMutex;
            std::set<SocketPtr> _RegisteredSockets;

            ContextContainer() = default;
            ContextContainer operator=(ContextContainer& Container);

            void EnqueueSocket(const SocketPtr& Ptr);
            void Erase(const SocketPtr& Ptr);
        };


        fd_set _ReadSet, _WriteSet;
        ContextContainer _Container;

    public:
        SelectIOContext(IOContextEvent &Event,
                        const uint32_t NumOfWorkerThread, const uint32_t Timeout);


    protected:
        virtual void DoWork ( ) override;

        virtual SocketPtr CreateSocket(socket_t Socket) override;


        void FDSet(ContextContainer& Container);
        void FDIsSetAndCallback(ContextContainer& Container);
    };



}


#endif //MUTENET_SELECTIOCONTEXT_HPP
