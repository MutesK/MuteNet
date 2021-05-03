//
// Created by junmkim on 2020-11-12.
//

#ifndef MUTENET_LIKEUNIXLISTENERCOMPONENT_H
#define MUTENET_LIKEUNIXLISTENERCOMPONENT_H

#ifdef POSIX_PLATFORM

#include "ListenerComponent.hpp"

namespace EventLoop
{
	class LikeUnixListenerComponent : public ListenerComponent
	{
		struct sockaddr* client_addr = nullptr;
		socklen_t clientAddresLength = 0;
	public:
		virtual ~LikeUnixListenerComponent();

		virtual void Disable() override;

		virtual void Free() override;

		friend class IUnixLikeIOContextImpl;

		friend class EpollContextImpl;

	private:
		LikeUnixListenerComponent(const RawIOContextImplPtr& ContextEvent,
			ListenerComponent::CallbackDelegate&& Callback,
			void* Self, socket_t listenSocket);

		static void ReadCallback(DescriptorPtr Ptr, void* Self);
		static void WriteCallback(DescriptorPtr Ptr, void* Self);
		static void ExceptCallback(DescriptorPtr Ptr, uint16_t What, void* Self);

		void Accept();
	};
}


#endif
#endif //MUTENET_LIKEUNIXLISTENERCOMPONENT_H
