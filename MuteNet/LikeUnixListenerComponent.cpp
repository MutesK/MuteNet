//
// Created by junmkim on 2020-11-12.
//

#ifdef POSIX_PLATFORM

#include "Common.h"
#include "TypeDefine.hpp"
#include "IoContextImpl.hpp"
#include "LikeUnixListenerComponent.h"
#include "IoContextThreadPool.hpp"

#include <fcntl.h>

namespace EventLoop
{
	LikeUnixListenerComponent::LikeUnixListenerComponent ( RawIOContextImplPtr const &ContextEvent,
														   ListenerComponent::CallbackDelegate &&Callback, void *Self,
														   descriptor_t listenSocket )
			: ListenerComponent ( ContextEvent, std::move(Callback), Self, listenSocket )
	{
	}

	LikeUnixListenerComponent::~LikeUnixListenerComponent ( )
	{
        Stop();

        close(_descriptor);
        _descriptor = INVALID_SOCKET;
	}

	void LikeUnixListenerComponent::Read()
	{
	}

	void LikeUnixListenerComponent::Write(void* data, size_t length)
	{
	}

	void LikeUnixListenerComponent::Enable()
	{
	}

	void LikeUnixListenerComponent::Disable(uint16_t Flag)
	{
	}

	bool LikeUnixListenerComponent::_Read()
	{
        if (_descriptor == INVALID_SOCKET || IsStop())
        {
            return false;
        }

		struct sockaddr_in client_addr;
		socklen_t client_addr_len = sizeof client_addr;

		descriptor_t client = accept(_descriptor,
			(struct sockaddr*)&client_addr, &client_addr_len);

		if (client == INVALID_SOCKET)
		{

		}

		// Non Block

		auto& ThreadPool = _ContextPtr->GetThreadPool();
		static const auto Dispatch = [&, client_addr=client_addr, client_addr_len= client_addr_len]()
		{
			_ListenCallbackDelegate((ListenerComponent *)this,
				static_cast<IUnixLikeIOContextImpl*>(_ContextPtr)->CreateDescriptor(client),
				(sockaddr *)&client_addr,
				client_addr_len,
				_Self);
		};
		ThreadPool->EnqueueJob(Dispatch);


		return true;
	}

	bool LikeUnixListenerComponent::_Write()
	{
		return true;
	}

    void LikeUnixListenerComponent::Start()
    {
        _Stop = false;

        static_cast<IUnixLikeIOContextImpl*>(_ContextPtr)->Enable(this);
    }

    void LikeUnixListenerComponent::Stop()
    {
        static_cast<IUnixLikeIOContextImpl*>(_ContextPtr)->Disable(this);

        _Stop = true;
    }
}

#endif
