#include "pch.h"
#include "Session.h"
#include "ServiceListener.h"

using namespace MuteNet;

SessionInfo* SessionInfo::NewSessionInfo(ServiceListener* Listener, intptr_t Fd)
{
	assert(Listener != nullptr);
	assert(Fd != INVALID_SOCKET);

	auto Info = new SessionInfo(Listener, Fd);
	return Info;
}


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

	// WSASend()
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
	WSABUF wsabuf[2];
	int bufcount;

	bufcount = _RecvBuffer.GetWriteBufferAndLengths(&wsabuf[0].buf, (uint32_t&)wsabuf[0].len,
		&wsabuf[1].buf, (uint32_t&)wsabuf[1].len);

	// WSARecv()
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

	Buffer.MoveWritePos(TransfferredBytes);
	Info->_RecvCb(Info, Info->_CompletionKey);

	Info->RecvPost();
}


