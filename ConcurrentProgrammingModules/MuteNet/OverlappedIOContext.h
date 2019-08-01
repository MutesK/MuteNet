#pragma once
#include <winbase.h>
#include "Link.h"


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
		IOType     Type;

		IOContext()
		{
			memset(&Overlapped, 0, sizeof(Overlapped));
			Type = IO_NONE;
		}
	};

	struct SendContext : IOContext
	{
		SendContext(const std::shared_ptr<Link> link)
			:IOContext()
		{
			Type = IO_SEND;
			linkPtr = link;
		}
	};

	struct RecvContext : IOContext
	{
		RecvContext(const std::shared_ptr<Link> link)
			:IOContext()
		{
			Type = IO_RECV;
			linkPtr = link;
		}
	};

	struct AcceptContext : IOContext
	{
		AcceptContext(const std::shared_ptr<Link> link)
			:IOContext()
		{
			Type = IO_ACCEPT;
			linkPtr = link;
		}
	};

	struct ConnectContext : IOContext
	{
		ConnectContext(const std::shared_ptr<Link> link)
			:IOContext()
		{
			Type = IO_CONNECT;
			linkPtr = link;
		}
	};

	struct DisconnectContext : IOContext
	{
		DisconnectContext(const std::shared_ptr<Link> link)
			:IOContext()
		{
			Type = IO_DISCONNECT;
			linkPtr = link;
		}
	};
	
}
