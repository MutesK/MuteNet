//
// Created by junmkim on 2020-11-12.
//

#ifndef MUTENET_LIKEUNIXLISTENERCOMPONENT_H
#define MUTENET_LIKEUNIXLISTENERCOMPONENT_H

#ifdef POSIX_PLATFORM

#include "ListenerComponent.hpp"
#include <Runnable.hpp>

namespace EventLoop
{
	class LikeUnixListenerComponent : public ListenerComponent, public Util::IRunnable
	{
	public:
		virtual ~LikeUnixListenerComponent();

        virtual void Start() override;

        virtual void Stop() override;

        friend class EpollContextImpl;
	private:
		LikeUnixListenerComponent(const RawIOContextImplPtr &ContextEvent,
								  ListenerComponent::CallbackDelegate &&Callback,
								  void *Self, socket_t listenSocket);

		// ListenerComponent을(를) 통해 상속됨
		virtual void Read() override;

		virtual void Write(void* data, size_t length) override;

		virtual void Enable() override;

		virtual void Disable(uint16_t Flag) override;

		virtual bool _Read() override;

		virtual bool _Write() override;
	};
}


#endif
#endif //MUTENET_LIKEUNIXLISTENERCOMPONENT_H
