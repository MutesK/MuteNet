#pragma once

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

		virtual ~IOContext() = default;
		virtual void IOComplete(DWORD TransfferedBytes, void* CompletionKey) = 0;
	};

	class SendContext : public IOContext
	{
	public:
		uint16_t SendCount;
		static Util::TL::ObjectPool<SendContext> OverlappedPool;

		SendContext(const std::shared_ptr<Link> link)
			:IOContext(link)
		{
			Type = IO_SEND;
		}

		void IOComplete(DWORD TransfferedBytes, void* CompletionKey) override;

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

		void IOComplete(DWORD TransfferedBytes, void* CompletionKey) override;
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

		void IOComplete(DWORD TransfferedBytes, void* CompletionKey) override;
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

		void IOComplete(DWORD TransfferedBytes, void* CompletionKey) override;
	};


}
