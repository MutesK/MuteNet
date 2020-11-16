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


	LikeUnixListenerComponent::~LikeUnixListenerComponent ( )
	{
        close(_ListenSocket);
	}

}

#endif
