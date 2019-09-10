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

	struct IOContext
	{
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

	struct SendContext : public IOContext
	{
		static Util::TL::ObjectPool<SendContext> OverlappedPool;

		SendContext(const std::shared_ptr<Link> link)
			:IOContext(link)
		{
			Type = IO_SEND;
		}

		void IOComplete(DWORD TransfferedBytes, void* CompletionKey) override;

	};

	struct RecvContext : IOContext
	{
		static Util::TL::ObjectPool<RecvContext> OverlappedPool;

		RecvContext(const std::shared_ptr<Link> link)
			:IOContext(link)
		{
			Type = IO_RECV;
		}

		void IOComplete(DWORD TransfferedBytes, void* CompletionKey) override;
	};

	struct AcceptContext : public IOContext
	{
		static Util::TL::ObjectPool<AcceptContext> OverlappedPool;

		AcceptContext(const std::shared_ptr<Link> link)
			:IOContext(link)
		{
			Type = IO_ACCEPT;
		}

		void IOComplete(DWORD TransfferedBytes, void* CompletionKey) override;
	};

	struct ConnectContext : public IOContext
	{
		static Util::TL::ObjectPool<ConnectContext> OverlappedPool;

		ConnectContext(const std::shared_ptr<Link> link)
			:IOContext(link)
		{
			Type = IO_CONNECT;
		}

		void IOComplete(DWORD TransfferedBytes, void* CompletionKey) override;
	};


}
