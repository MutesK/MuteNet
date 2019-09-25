#include "pch.h"
#include "Link.h"
#include "IOContext.h"



using namespace Util;
namespace Network
{
	Link::Link()
	{
		_socket = std::make_unique<TcpSocket>(AF_INET);

		_RecvQ = CircularBuffer::Alloc();
		_SendQ = CircularBuffer::Alloc();
	}

	Link::~Link()
	{
		std::cout << "Destory LINK ERROR!!!!!\n";
	}

	void Link::RecvPost()
	{
		const auto Overlapped = RecvContext::OverlappedPool(shared_from_this());

		WSABUF wsabuf[2];
		int bufcount;

		bufcount = _RecvQ->GetWriteBufferAndLengths((void**)&wsabuf[0].buf, (size_t&)wsabuf[0].len,
			(void**)&wsabuf[1].buf, (size_t&)wsabuf[1].len);

		_socket->OverlappedIORecv(wsabuf, bufcount, &Overlapped->Overlapped);
	}

	void Link::SendPost()
	{
		const auto Overlapped = SendContext::OverlappedPool(shared_from_this());

		WSABUF wsabuf[2];
		int bufcount;

		{
			std::lock_guard<std::recursive_mutex> lock(_SendQ->_mutex);

			bufcount = _SendQ->GetReadBufferAndLengths((void **)&wsabuf[0].buf, (size_t&)wsabuf[0].len,
				(void **)&wsabuf[1].buf, (size_t&)wsabuf[1].len);
		}

		_socket->OverlappedIOSend(wsabuf, bufcount, &Overlapped->Overlapped);

	}

	void Link::SendPacket(const std::shared_ptr<Util::OutputMemoryStream>& Packet)
	{
		std::lock_guard<std::recursive_mutex> lock(_SendQ->_mutex);

		uint32_t size = Packet->GetLength();
		_SendQ->PutData(&size, sizeof(uint32_t));
		_SendQ->PutData(Packet->GetBufferPtr(), Packet->GetLength());

		SendPost();
	}


}
