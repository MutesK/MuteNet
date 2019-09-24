#pragma once

namespace Network
{
	struct IOConext;
	class Link;

	enum IOType
	{
		IO_NONE,
		IO_RECV,
		IO_SEND,
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

		~IOContext() = default;
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

}
