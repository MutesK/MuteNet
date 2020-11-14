//
// Created by JunMin Kim on 2020/11/11.
//

#ifndef MUTENET_EVENTBASECOMPONENT_HPP
#define MUTENET_EVENTBASECOMPONENT_HPP

namespace EventLoop
{
	class IOContextImpl;
	
	using RawIOContextImplPtr = IOContextImpl *;
	
	class IEventBaseComponent
	{
	protected:
		RawIOContextImplPtr _ContextPtr;
		
		IEventBaseComponent ( const RawIOContextImplPtr &Ptr );
	};
	
}


#endif //MUTENET_EVENTBASECOMPONENT_HPP
