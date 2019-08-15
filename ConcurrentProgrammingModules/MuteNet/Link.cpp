
#include "Link.h"
#include "IOContext.h"
#include "../MemoryStream/BufferPool.h"
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

		bufcount = _RecvQ->GetWrtieBufferPtr(wsabuf[0].buf, reinterpret_cast<size_t&>(wsabuf[0].len),
			wsabuf[1].buf, reinterpret_cast<size_t &>(wsabuf[1].len));

		_Socket.OverlappedIORecv(wsabuf, bufcount, &Overlapped->Overlapped);
	}

	void Link::SendPost()
	{
		const auto Overlapped = SendContext::OverlappedPool(shared_from_this());

		_Socket.OverlappedIOSend(nullptr, 0, &Overlapped->Overlapped);
	}

}
