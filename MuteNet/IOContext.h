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

		IOContext(const std::shared_ptr<Link> link)
			:linkPtr(link)
		{
			memset(&Overlapped, 0, sizeof(OVERLAPPED));
			Type = IO_NONE;
		}
	};

	class SendContext : public IOContext
	{
	public:
		static Util::TL::ObjectPool<SendContext> OverlappedPool;

		SendContext(const std::shared_ptr<Link> link)
			:IOContext(link)
		{
			Type = IO_SEND;
		}
		~SendContext() = default;

		void IOComplete(DWORD TransfferedBytes, void* CompletionKey);

	};

	class RecvContext : public IOContext
	{
	public:
		static Util::TL::ObjectPool<RecvContext> OverlappedPool;

		RecvContext(const std::shared_ptr<Link> link)
			:IOContext(link)
		{
			Type = IO_RECV;
		}
		~RecvContext() = default;

		void IOComplete(DWORD TransfferedBytes, void* CompletionKey);
	};

	class AcceptContext : public IOContext
	{
	public:
		static Util::TL::ObjectPool<AcceptContext> OverlappedPool;

		AcceptContext(const std::shared_ptr<Link> link)
			:IOContext(link)
		{
			Type = IO_ACCEPT;
		}
		~AcceptContext() = default;

		void IOComplete(DWORD TransfferedBytes, void* CompletionKey, IOService* Service);
	};

	class ConnectContext : public IOContext
	{
	public:
		static Util::TL::ObjectPool<ConnectContext> OverlappedPool;

		ConnectContext(const std::shared_ptr<Link> link)
			:IOContext(link)
		{
			Type = IO_CONNECT;
		}
		~ConnectContext() = default;

		void IOComplete(DWORD TransfferedBytes, void* CompletionKey);
	};


}
