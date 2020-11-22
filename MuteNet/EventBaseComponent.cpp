//
// Created by Mute on 2020-11-22.
//

#include "Common.h"
#include "TypeDefine.hpp"
#include "IoContextImpl.hpp"
#include "EventBaseComponent.hpp"

namespace EventLoop
{
    IEventBaseComponent::IEventBaseComponent ( const RawIOContextImplPtr &Ptr )
            : _ContextPtr (reinterpret_cast<RawIOContextImplPtr *>(const_cast<RawIOContextImplPtr>(Ptr)))
    {
    }
}