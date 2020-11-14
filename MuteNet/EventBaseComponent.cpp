//
// Created by JunMin Kim on 2020/11/11.
//

#include "Common.h"
#include "EventBaseComponent.hpp"
#include "IoContextEvent.hpp"

namespace EventLoop
{
	IEventBaseComponent::IEventBaseComponent ( const RawIOContextImplPtr &Ptr )
			: _ContextPtr ( Ptr )
	{
	}
	
}