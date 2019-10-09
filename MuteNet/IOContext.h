#pragma once

namespace MuteNet
{
	struct IOConext;
	class Link;
	class IOService;

	enum IOType
	{
		IO_NONE,
		IO_ACCEPT,  // Acceptor
		IO_RECV,
		IO_SEND,
		IO_CONNECT,  // Connector
	};

	class IOContext
	{
	public:
		OVERLAPPED Overlapped;
		Link* linkPtr;
		IOType     Type;

		IOContext()
		{
			memset(&Overlapped, 0, sizeof(OVERLAPPED));
			Type = IO_NONE;
		}
	};

	class SendContext : public IOContext
	{
	public:

		SendContext()
		{
			Type = IO_SEND;
		}
	};

	class RecvContext : public IOContext
	{
	public:

		RecvContext()
		{
			Type = IO_RECV;
		}
	};

	class AcceptContext : public IOContext
	{
	public:
		AcceptContext()
		{
			Type = IO_ACCEPT;
		}
	};

	class ConnectContext : public IOContext
	{
	public:
		ConnectContext()
		{
			Type = IO_CONNECT;
		}
	};


}
