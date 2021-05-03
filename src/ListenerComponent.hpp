//
// Created by JunMin Kim on 2020/11/11.
//

#ifndef MUTENET_LISTENERCOMPONENT_HPP
#define MUTENET_LISTENERCOMPONENT_HPP

#include "EventBaseComponent.hpp"

namespace EventLoop
{
	class IDescriptor;
	using DescriptorPtr = IDescriptor*;

	class ListenerComponent : public IEventBaseComponent
	{
	public:
		using CallbackDelegate = void (*)(ListenerComponent* pRawListener, DescriptorPtr clientsocket,
			sockaddr* address, int length, void* Self);
	protected:
		DescriptorPtr   _Listener;
		CallbackDelegate _ListenCallbackDelegate;
		void* _Self;

		friend class IOContextImpl;

	public:
		virtual ~ListenerComponent();

		virtual void Disable() = 0;
		virtual void Free() = 0;

	protected:
		ListenerComponent(const RawIOContextImplPtr& ContextEvent,
			CallbackDelegate&& Callback,
			void* Self, descriptor_t listenSocket);

	};
}

#endif //MUTENET_LISTENERCOMPONENT_HPP
