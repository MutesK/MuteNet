
#include "Link.h"
#include "IOContext.h"
#include "../RingBuffer/CircularBuffer.h"



using namespace Util;
namespace Network
{
	Link::Link()
		:_Socket(AF_INET)
	{

		_RecvQ = CircularBuffer::Alloc();
		_SendQ = CircularBuffer::Alloc();
	}

	Link::~Link()
	{
	}

	void Link::RecvPost()
	{
		const auto Overlapped = RecvContext::OverlappedPool(shared_from_this());

		WSABUF wsabuf[2];
		int bufcount;

		bufcount = _RecvQ->GetWriteBufferAndLengths(wsabuf[0].buf, reinterpret_cast<size_t&>(wsabuf[0].len),
			wsabuf[1].buf, reinterpret_cast<size_t &>(wsabuf[1].len));

		_socket.OverlappedIORecv(wsabuf, bufcount, &Overlapped->Overlapped);
	}

	void Link::SendPost()
	{
		const auto Overlapped = SendContext::OverlappedPool(shared_from_this());

		WSABUF wsabuf[2];
		int bufcount;

		{
			std::lock_guard<std::recursive_mutex> lock(_SendQ->_mutex);

			bufcount = _SendQ->GetReadBufferAndLengths(wsabuf[0].buf, reinterpret_cast<size_t&>(wsabuf[0].len),
				wsabuf[1].buf, reinterpret_cast<size_t&>(wsabuf[1].len));
		}

		_isSend = true;

		_socket.OverlappedIOSend(wsabuf, bufcount, &Overlapped->Overlapped);

	}

}
