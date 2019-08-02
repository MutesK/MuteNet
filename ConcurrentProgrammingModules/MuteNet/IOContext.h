#pragma once
#include <winbase.h>
#include "Link.h"
#include "../ObjectPool/ObjectPool.h"
#include "../Singleton/Singleton.h"


namespace Network
{
	enum IOType
	{
		IO_NONE,
		IO_ACCEPT,
		IO_RECV,
		IO_SEND,
		IO_CONNECT,
		IO_DISCONNECT,
	};

	struct IOContext
	{
		OVERLAPPED Overlapped;
		std::shared_ptr<Link> linkPtr;
		std::function<void(IOContext*)> Callback;
		IOType     Type;

		IOContext()
		{
			memset(&Overlapped, 0, sizeof(Overlapped));
			Type = IO_NONE;
		}
	};

	struct SendContext : public IOContext
	{
		static Util::TL::ObjectPool<SendContext> OverlappedPool;

		SendContext(const std::shared_ptr<Link> link)
		{
			Type = IO_SEND;
			linkPtr = link;
		}
	};

	struct RecvContext : IOContext
	{
		static Util::TL::ObjectPool<RecvContext> OverlappedPool;

		RecvContext(const std::shared_ptr<Link> link)
		{
			Type = IO_RECV;
			linkPtr = link;
		}
	};

	struct AcceptContext : public IOContext
	{
		static Util::TL::ObjectPool<AcceptContext> OverlappedPool;

		AcceptContext(const std::shared_ptr<Link> link)
		{
			Type = IO_ACCEPT;
			linkPtr = link;
		}
	};

	struct ConnectContext : IOContext
	{
		static Util::TL::ObjectPool<ConnectContext> OverlappedPool;

		ConnectContext(const std::shared_ptr<Link> link)
		{
			Type = IO_CONNECT;
			linkPtr = link;
		}
	};

	struct DisconnectContext : IOContext
	{
		static Util::TL::ObjectPool<DisconnectContext> OverlappedPool;

		DisconnectContext(const std::shared_ptr<Link> link)
		{
			Type = IO_DISCONNECT;
			linkPtr = link;
		}
	};

}
