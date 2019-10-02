#pragma once

namespace Network
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
		std::shared_ptr<Link> linkPtr;
		IOType     Type;

		IOContext(std::shared_ptr<Link> link)
			:linkPtr(link)
		{
			memset(&Overlapped, 0, sizeof(OVERLAPPED));
			Type = IO_NONE;
		}
	};

	class SendContext : public IOContext
	{
	public:

		SendContext(std::shared_ptr<Link> link)
			:IOContext(link)
		{
			Type = IO_SEND;
		}

		void IOComplete(DWORD TransfferedBytes, void* CompletionKey);

	};

	class RecvContext : public IOContext
	{
	public:

		RecvContext(std::shared_ptr<Link> link)
			:IOContext(link)
		{
			Type = IO_RECV;
		}

		void IOComplete(DWORD TransfferedBytes, void* CompletionKey);
	};

	class AcceptContext : public IOContext
	{
	public:

		AcceptContext(std::shared_ptr<Link> link)
			:IOContext(link)
		{
			Type = IO_ACCEPT;
		}

		void IOComplete(DWORD TransfferedBytes, void* CompletionKey, IOService* Service);
	};

	class ConnectContext : public IOContext
	{
	public:

		ConnectContext(std::shared_ptr<Link> link)
			:IOContext(link)
		{
			Type = IO_CONNECT;
		}

		void IOComplete(DWORD TransfferedBytes, void* CompletionKey);
	};


}
