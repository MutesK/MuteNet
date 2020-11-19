//
// Created by JunMin Kim on 2020/11/11.
//

#include "Common.h"
#include "TypeDefine.hpp"
#include "IocpContextImpl.hpp"
#include "ListenerComponent.hpp"
#include "Descriptor.h"

namespace EventLoop
{
	ListenerComponent::ListenerComponent ( const RawIOContextImplPtr &ContextEvent,
	                                       ListenerComponent::CallbackDelegate &&Callback, void *Self,
	                                       descriptor_t listenSocket )
			: IEventBaseComponent ( ContextEvent ),
			  _ListenCallbackDelegate ( Callback ),
			  _Self ( Self ),
              _Listener(ContextEvent->CreateDescriptor(listenSocket))
	{

	}
	
	ListenerComponent::~ListenerComponent ( )
	{
	
	}


}