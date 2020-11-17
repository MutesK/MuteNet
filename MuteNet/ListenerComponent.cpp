//
// Created by JunMin Kim on 2020/11/11.
//

#include "Common.h"
#include "TypeDefine.hpp"
#include "ListenerComponent.hpp"

namespace EventLoop
{
	ListenerComponent::ListenerComponent ( const RawIOContextImplPtr &ContextEvent,
	                                       ListenerComponent::CallbackDelegate &&Callback, void *Self,
	                                       descriptor_t listenSocket )
			: IDescriptor ( ContextEvent, listenSocket  ),
			  _ListenCallbackDelegate ( Callback ),
			  _Self ( Self )
	{
	
	}
	
	ListenerComponent::~ListenerComponent ( )
	{
	
	}

}