#pragma once

#include "pch.h"
#include "ASyncWork.h"
#include "Link.h"
#include "IOContext.h"
#include "../Foundation/CircularBuffer.h"
#include "TcpSocket.h"

namespace Network
{
	class Link;

	template <class ContextType>
	class ASyncWork final
	{
	public:
		static void WorkRequest(const std::shared_ptr<Link>& Link);
	};

	static void SendPacket(const std::shared_ptr<Link>& Link, const std::shared_ptr<Util::OutputMemoryStream>& Packet);

	template<class ContextType>
	void ASyncWork<ContextType>::WorkRequest(const std::shared_ptr<Link>& link)
	{
		const auto Overlapped = new ContextType(link);
		WSABUF wsabuf[2];
		int bufcount;

		const auto socket = link->get_socket();

		switch (Overlapped->Type)
		{
		case IO_RECV:
		{
			const auto RecvQ = link->get_RecvQ();

			bufcount = RecvQ->GetWriteBufferAndLengths(&wsabuf[0].buf, (uint32_t &)wsabuf[0].len,
				&wsabuf[1].buf, (uint32_t&)wsabuf[1].len);

			socket->OverlappedIORecv(wsabuf, bufcount, &Overlapped->Overlapped);
		}
		break;
		case IO_SEND:
		{
			const auto SendQ = link->get_SendQ();

			while(SendQ->get_isSending())
				continue;

			SendQ->set_isSending(true);

			{
				SAFE_SHAREDLOCK(SendQ->GetSharedLock());

				bufcount = SendQ->GetReadBufferAndLengths(&wsabuf[0].buf, (uint32_t&)wsabuf[0].len,
					&wsabuf[1].buf, (uint32_t&)wsabuf[1].len);
			}

			socket->OverlappedIOSend(wsabuf, bufcount, &Overlapped->Overlapped);
		}
		break;
		}
	}
	
	void SendPacket(const std::shared_ptr<Link>& Link, const std::shared_ptr<Util::OutputMemoryStream>& Packet)
	{
		const auto SendQ = Link->get_SendQ();

		{
			SAFE_UNIQUELOCK(SendQ->GetSharedLock());

			SendQ->PutData(Packet->GetBufferPtr(), Packet->GetLength());
		}

		ASyncWork<SendContext>::WorkRequest(Link);
	}
}


