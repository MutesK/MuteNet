//
// Created by JunMin Kim on 2020/11/11.
//

#ifndef MUTENET_IOCONTEXTEVENT_HPP
#define MUTENET_IOCONTEXTEVENT_HPP

#include "ListenerComponent.hpp"

namespace EventLoop
{
	class IOContextImpl;
	using IOContextImplPtr = std::shared_ptr<IOContextImpl>;

	class ListenerComponent;
	using ListenerPtr = std::shared_ptr<ListenerComponent>;

	class IDescriptor;
	using DescriptorPtr = IDescriptor*;

	class IOContextEvent final
	{
		IOContextImplPtr _ContextImpl;
	public:
		IOContextEvent(const uint32_t NumOfWorkerThread, const uint32_t Timeout);

		~IOContextEvent();

		ListenerPtr CreateListener(ListenerComponent::CallbackDelegate Callback,
			void* Self, descriptor_t listenSocket);

		DescriptorPtr CreateDescriptor(descriptor_t descriptor);

		bool Enable(const DescriptorPtr descriptor);
		void Disable(const DescriptorPtr descriptor);
	};
}


#endif //MUTENET_IOCONTEXTEVENT_HPP
