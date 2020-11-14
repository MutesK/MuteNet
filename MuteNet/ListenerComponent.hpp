//
// Created by JunMin Kim on 2020/11/11.
//

#ifndef MUTENET_LISTENERCOMPONENT_HPP
#define MUTENET_LISTENERCOMPONENT_HPP

#include "EventBaseComponent.hpp"

namespace EventLoop
{
	class ISocketDescriptor;
	
	using SocketPtr = std::shared_ptr<ISocketDescriptor>;
	
	class IOContextImpl;
	
	class ListenerComponent : public IEventBaseComponent
	{
	public:
		using CallbackDelegate = std::function<void ( ListenerComponent *pRawListener, SocketPtr clientsocket,
		                                              sockaddr *address, int length, void *Self )>;
	protected:
		socket_t _ListenSocket;
		CallbackDelegate _ListenCallbackDelegate;
		void *_Self;
		uint32_t _Flag;
		int _Backlog;
		
		friend class IOContextImpl;
	
	public:
		virtual ~ListenerComponent ( );
	
	protected:
		ListenerComponent ( const RawIOContextImplPtr &ContextEvent,
		                    CallbackDelegate &&Callback,
		                    void *Self, uint32_t Flag, int backlog, socket_t listenSocket );
	};
}

#endif //MUTENET_LISTENERCOMPONENT_HPP
