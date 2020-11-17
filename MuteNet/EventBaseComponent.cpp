//
// Created by JunMin Kim on 2020/11/11.
//

#include "Common.h"
#include "IocpContextImpl.hpp"
#include "EventBaseComponent.hpp"

namespace EventLoop
{
	IEventBaseComponent::IEventBaseComponent ( const RawIOContextImplPtr &Ptr )
			: _ContextPtr (reinterpret_cast<RawIOContextImplPtr *>(const_cast<RawIOContextImplPtr>(Ptr)))
	{
	}
	
}