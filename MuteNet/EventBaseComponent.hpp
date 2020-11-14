//
// Created by JunMin Kim on 2020/11/11.
//

#ifndef MUTENET_EVENTBASECOMPONENT_HPP
#define MUTENET_EVENTBASECOMPONENT_HPP

#include "IocpContextImpl.hpp"
#include "SelectIOContext.hpp"

namespace EventLoop
{
	class IOContextImpl;
	
	using RawIOContextImplPtr = IOContextImpl *;
	
	class IEventBaseComponent
	{
	protected:
		EventLoop::SelectIOContext *_ContextPtr;
		
		IEventBaseComponent ( const RawIOContextImplPtr &Ptr );
	};
	
}


#endif //MUTENET_EVENTBASECOMPONENT_HPP
