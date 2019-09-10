
#include "Link.h"
#include "IOContext.h"
#include "Acceptor.h"
#include "EngineIO.h"
#include "../RingBuffer/CircularBuffer.h"
#include "../MemoryStream/OutputByteStream.h"

using namespace Util;

namespace Network
{
	Util::TL::ObjectPool<SendContext> SendContext::OverlappedPool;
	Util::TL::ObjectPool<RecvContext> RecvContext::OverlappedPool;
	Util::TL::ObjectPool<AcceptContext> AcceptContext::OverlappedPool;
	Util::TL::ObjectPool<ConnectContext> ConnectContext::OverlappedPool;

	void SendContext::IOComplete(DWORD TransfferedBytes, void* CompletionKey)
	{
		auto& SendQ = linkPtr->get_SendQ();

		{
			std::lock_guard<std::recursive_mutex> lock(SendQ->_mutex);

			SendQ->MoveReadPostion(TransfferedBytes);
		}

		EngineIO::OnSended(linkPtr, TransfferedBytes);

		linkPtr->get_isSend() = true;
		SendContext::OverlappedPool.Free(this);
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
			
			uint32_t length = 0;

			while (true)
			{
				length = 0;
				RecvQ->GetData(&length, sizeof(length));

				if (RecvQ->GetUsedSize() < length)
					break;

				const auto bufferPtr = RecvQ->GetReadBufferPtr();
				
				auto bufferStream = std::make_shared<InputMemoryStream>(bufferPtr, length);
				RecvQ->MoveReadPostion(length);

				EngineIO::OnRecived(linkPtr, bufferStream);
			}


			linkPtr->RecvPost();
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
		ConnectPoint::Setter::SetConnectionPoint(LinkSocket, linkPtr->get_EndPoint());

		// Register Socket To IOCP

		EngineIO::OnAccepted(linkPtr);
		
		AcceptContext::OverlappedPool.Free(this);
	}

	void ConnectContext::IOComplete(DWORD TransfferedBytes, void* CompletionKey)
	{

	}
}