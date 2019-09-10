
#include "Link.h"
#include "IOContext.h"
#include "Acceptor.h"
#include "EngineIO.h"
#include "../RingBuffer/CircularBuffer.h"

namespace Network
{
	Util::TL::ObjectPool<SendContext> SendContext::OverlappedPool;
	Util::TL::ObjectPool<RecvContext> RecvContext::OverlappedPool;
	Util::TL::ObjectPool<AcceptContext> AcceptContext::OverlappedPool;
	Util::TL::ObjectPool<ConnectContext> ConnectContext::OverlappedPool;

	void SendContext::IOComplete(DWORD TransfferedBytes, void* CompletionKey)
	{

	}

	void RecvContext::IOComplete(DWORD TransfferedBytes, void* CompletionKey)
	{
		if (TransfferedBytes <= 0)
		{
			linkPtr->get_socket().Shutdown(ShutdownBlockMode::BothBlock);
		}
		else
		{
			auto& RecvQ = linkPtr->get_RecvQ();

			RecvQ->MoveWritePostion(TransfferedBytes);

			// Packet Tokenizer and Notify
		}

		RecvContext::OverlappedPool.Free(this);
	}

	void AcceptContext::IOComplete(DWORD TransfferedBytes, void* CompletionKey)
	{
		auto ListenPtr = 
			*static_cast<std::shared_ptr<TcpSocket>*>(CompletionKey);
		
		auto LinkSocket = linkPtr->get_socket();

		LinkSocket.SetUpdateAcceptContext(ListenPtr->socket_handle());
		LinkSocket.SetNagle(true);
		ConnectPoint::Setter::SetConnectionPoint(LinkSocket,
			linkPtr->get_EndPoint());

		EngineIO::OnAccepted(linkPtr);
		
		AcceptContext::OverlappedPool.Free(this);
	}

	void ConnectContext::IOComplete(DWORD TransfferedBytes, void* CompletionKey)
	{

	}
}