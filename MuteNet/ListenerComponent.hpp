//
// Created by JunMin Kim on 2020/11/11.
//

#ifndef MUTENET_LISTENERCOMPONENT_HPP
#define MUTENET_LISTENERCOMPONENT_HPP

#include "EventBaseComponent.hpp"

namespace EventLoop
{
    class ListenerComponent : public IEventBaseComponent
    {
    public:
        using CallbackDelegate = std::function<void(ListenerComponent *pRawListener, socket_t listenSocket,
                                                    sockaddr *address, int length, void *Self)>;
    private:
        socket_t _ListenSocket;
        CallbackDelegate _ListenCallbackDelegate;
        void *_Self;
        uint32_t _Flag;
        int _Backlog;
    public:
        static std::shared_ptr<ListenerComponent> New(const IOContextImplPtr &ContextEvent,
                                                      CallbackDelegate &&Callback,
                                                      void *Self, uint32_t Flag, int backlog, socket_t listenSocket);

        virtual ~ListenerComponent();

    private:
        ListenerComponent(const IOContextImplPtr &ContextEvent,
                          CallbackDelegate &&Callback,
                          void *Self, uint32_t Flag, int backlog, socket_t listenSocket);
    };
}

#endif //MUTENET_LISTENERCOMPONENT_HPP
