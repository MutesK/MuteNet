//
// Created by junmkim on 2020-11-12.
//

#if defined(APPLE) || defined(LINUX) || defined(UNIXLIKE)

#include "Common.h"
#include "TypeDefine.hpp"
#include "IoContextImpl.hpp"
#include "LikeUnixListenerComponent.h"
#include "SelectIOContext.hpp"

namespace EventLoop
{
	LikeUnixListenerComponent::LikeUnixListenerComponent ( RawIOContextImplPtr const &ContextEvent,
														   ListenerComponent::CallbackDelegate &&Callback, void *Self,
														   uint32_t Flag, int backlog, socket_t listenSocket )
			: ListenerComponent ( ContextEvent, std::move(Callback), Self, Flag, backlog, listenSocket )
	{
	}

	void LikeUnixListenerComponent::DoWork ( )
	{
		struct sockaddr address;
		socklen_t addresslength = sizeof(address);

		if(-1 == getsockname(_ListenSocket, &address, &addresslength))
		{
			return;
		}

		struct sockaddr* clientAddress;
		socklen_t clientAddressLength = 0;

		switch(address.sa_family)
		{
			case AF_INET:
				clientAddress = reinterpret_cast<struct sockaddr *>(new sockaddr_in());
				clientAddressLength = sizeof(sockaddr_in);
				break;
			case AF_INET6:
				clientAddress = reinterpret_cast<struct sockaddr *>(new sockaddr_in6());
				clientAddressLength = sizeof(sockaddr_in6);
				break;
		}

		const auto& Pool = reinterpret_cast<IOContextImpl *>(_ContextPtr)->GetThreadPool();
		socket_t clientsocket = INVALID_SOCKET;
		while(!IsStop())
		{
			clientsocket = accept ( _ListenSocket,
											 reinterpret_cast<struct sockaddr *>(&clientAddress),
											 &clientAddressLength );
			if ( -1 == clientsocket )
			{
				continue;
			}

			Pool->EnqueueJob([&]()
							 {
								 _ListenCallbackDelegate(this, _ContextPtr->CreateSocket(clientsocket),
														 clientAddress, clientAddressLength, _Self);
							 });
		}

		delete clientAddress;
	}

	LikeUnixListenerComponent::~LikeUnixListenerComponent ( )
	{
		Stop();
	}

}

#endif
