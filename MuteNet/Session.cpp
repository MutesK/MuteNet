#include "pch.h"
#include "Session.h"
#include "ServiceListener.h"

using namespace MuteNet;

void SessionInfo::Enable()
{
	_EventBase->RegisterHandle(
		reinterpret_cast<void*>(_Socket), this);

	RecvPost();
}

void SessionInfo::Disable()
{
	closesocket(_Socket);
}

void SessionInfo::Shutdown()
{
	shutdown(_Socket, SD_SEND);
}

void SessionInfo::SetCallback(RecvCallback Recv, SendCallback Send, void* Key)
{
	assert(Recv != nullptr);
	assert(Send != nullptr);

	_RecvCb = Recv;
	_SendCb = Send;

	_CompletionKey = Key;
}


void SessionInfo::Send(const void* Data, size_t Length)
{
	WSABUF wsabuf[2];
	int bufcount;

	while (_SendBuffer.get_isSending())
		continue;

	_SendBuffer.set_isSending(true);

	{
		SAFE_SHAREDLOCK(_SendBuffer.GetSharedLock());

		bufcount = _SendBuffer.GetReadBufferAndLengths(&wsabuf[0].buf, (uint32_t&)wsabuf[0].len,
			&wsabuf[1].buf, (uint32_t&)wsabuf[1].len);
	}

	DWORD SendBytes = 0;
	auto ret = ::WSASend(_Socket, wsabuf, bufcount, &SendBytes,
		0, &_Send._OverlappedIO, nullptr);
	if (ret == SOCKET_ERROR)
	{
		int errorCode = WSAGetLastError();
		
		if (errorCode != WSA_IO_PENDING)
		{
			// Event Callback
			return;
		}
	}
}

int SessionInfo::Connect(const sockaddr* Ip, int Size)
{
	return ::connect(_Socket, Ip, Size);
}

SessionInfo::SessionInfo(ServiceListener* Listener, SOCKET Fd)
	:_EventBase(Listener), _Socket(Fd), _RecvCb(nullptr), _SendCb(nullptr), _CompletionKey(nullptr)
	, _Send{ OVERLAPPED(), OnSend }, _Recv{ OVERLAPPED(), OnRecv }
{
}

void SessionInfo::RecvPost()
{
	// Copy-Zero-bytes (Non Pagelocking)
	DWORD RecvBytes = 0;

	int ret = WSARecv(_Socket, nullptr, 0, &RecvBytes, nullptr, &_Recv._OverlappedIO,
		nullptr);
	if (ret == SOCKET_ERROR)
	{
		int errorCode = WSAGetLastError();

		if (errorCode != WSA_IO_PENDING)
		{
			// Event Callback
			return;
		}
	}
}

void SessionInfo::OnSend(DWORD TransfferredBytes, void* InfoPtr)
{
	auto Info = reinterpret_cast<SessionInfo*>(InfoPtr);
	auto& Buffer = Info->_SendBuffer;

	Buffer.MoveReadPostion(TransfferredBytes);
	Buffer.set_isSending(false);

	Info->_SendCb(Info, Info->_CompletionKey);
}

void SessionInfo::OnRecv(DWORD TransfferredBytes, void* InfoPtr)
{
	auto Info = reinterpret_cast<SessionInfo*>(InfoPtr);
	auto& Buffer = Info->_RecvBuffer;

	WSABUF wsabuf[2];
	int bufcount;
	bufcount = Buffer.GetWriteBufferAndLengths(&wsabuf[0].buf, (uint32_t&)wsabuf[0].len,
		&wsabuf[1].buf, (uint32_t&)wsabuf[1].len);

	// ::recv Ã³¸®
	auto bufsize = Info->RecvProcess(wsabuf[0].buf, wsabuf[0].len);
	bufsize += Info->RecvProcess(wsabuf[1].buf, wsabuf[1].len);

	Info->_RecvCb(Info, Info->_CompletionKey);
	Info->RecvPost();

}

int MuteNet::SessionInfo::RecvProcess(char* bufferPos, int length)
{
	if (length <= 0)
		return;

	int result = ::recv(_Socket, bufferPos, length, 0);
	if (result == SOCKET_ERROR)
	{
		// Event Callback
		return;
	}

	_RecvBuffer.MoveWritePos(result);
	return result;
}


