#pragma once

#include "MuteNetFoundation.h"

namespace Network
{
	struct IOConext;
	class Link;

	enum IOType
	{
		IO_NONE,
		IO_ACCEPT,  // Acceptor
		IO_RECV,
		IO_SEND,
		IO_CONNECT,  // Connector
	};

	using ContextCallback = std::function<void(struct IOContext*, DWORD, void*)>;
	struct IOContext
	{
		OVERLAPPED Overlapped;
		std::shared_ptr<Link> linkPtr;
		ContextCallback& Callback;
		IOType     Type;

		IOContext(const std::shared_ptr<Link> link, ContextCallback& Callback)
			:linkPtr(link), Callback(Callback)
		{
			memset(&Overlapped, 0, sizeof(OVERLAPPED));
			Type = IO_NONE;
		}
	};

	struct SendContext : public IOContext
	{
		static Util::TL::ObjectPool<SendContext> OverlappedPool;

		SendContext(const std::shared_ptr<Link> link, ContextCallback& Callback)
			:IOContext(link, Callback)
		{
			Type = IO_SEND;
		}

	};

	struct RecvContext : IOContext
	{
		static Util::TL::ObjectPool<RecvContext> OverlappedPool;

		RecvContext(const std::shared_ptr<Link> link, ContextCallback& Callback)
			:IOContext(link, Callback)
		{
			Type = IO_RECV;
		}
	};

	struct AcceptContext : public IOContext
	{
		static Util::TL::ObjectPool<AcceptContext> OverlappedPool;

		AcceptContext(const std::shared_ptr<Link> link, ContextCallback& Callback)
			:IOContext(link, Callback)
		{
			Type = IO_ACCEPT;
		}
	};

	struct ConnectContext : IOContext
	{
		static Util::TL::ObjectPool<ConnectContext> OverlappedPool;

		ConnectContext(const std::shared_ptr<Link> link, ContextCallback& Callback)
			:IOContext(link, Callback)
		{
			Type = IO_CONNECT;
		}
	};


}
