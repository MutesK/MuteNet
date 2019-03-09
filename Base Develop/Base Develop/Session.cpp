#include "TcpSocket.h"
#include "UdpSocket.h"
#include "Session.h"


Session::Session(TcpSocketPtr& socket)
: _socket(socket)
{
	_SendQ = RingBuffer::GetInstance()->get_shared();
	_RecvQ = RingBuffer::GetInstance()->get_shared();
}


Session::~Session()
{
	_SendQ->ClearBuffer();
	_RecvQ->ClearBuffer();

	_socket->shutdown(ShutdownBlockMode::BothBlock);
}


bool Session::RecvPost()
{
	if (_recvFlag)
		return false;

	memset(&_recvOverlapped, 0, sizeof(OVERLAPPED));

	int bufcount = 1;

	WSABUF wsabuf[2];
	wsabuf[0].buf = _RecvQ->GetWriteBufferPtr();
	wsabuf[0].len = _RecvQ->GetNotBrokenPutSize();

	if (wsabuf[0].len < _RecvQ->GetFreeSize())
	{
		bufcount++;
		wsabuf[1].buf = _RecvQ->GetBufferPtr();
		wsabuf[1].len = _RecvQ->GetFreeSize() - wsabuf[0].len;
	}

	int result = _socket->OverlappedIORecv(wsabuf, bufcount, &_recvOverlapped);
	
	if (result == -1)
		return false;

	_recvFlag = true;

	return true;
}

bool Session::SendPost()
{
	if (_SendQ->GetUseSize() <= 0 || _sendFlag)
		return false;

	memset(&_sendOverlapped, 0, sizeof(OVERLAPPED));

	WSABUF wsabuf[2];
	int bufcount = 1;

	wsabuf[0].buf = _SendQ->GetReadBufferPtr();
	wsabuf[0].len = _SendQ->GetNotBrokenGetSize();

	if (_SendQ->GetUseSize() > wsabuf[0].len)
	{
		bufcount++;
		wsabuf[1].buf = _SendQ->GetBufferPtr();
		wsabuf[1].len = _SendQ->GetUseSize() - wsabuf[0].len;
	}

	int result = _socket->OverlappedIOSend(wsabuf, bufcount, &_sendOverlapped);

	if (result == -1)
		return false;

	_sendFlag = true;

	return true;

}

void Session::SendPacket(std::shared_ptr<StreamBuffer> buffer)
{
	_SendQ->PutData(buffer->GetBufferPtr(), buffer->GetUseSize());

	SendPost();
}

bool Session::RecvCompletionIO(DWORD Transferred)
{
	{
		std::lock_guard<std::mutex> lock(_recvMutex);

		_RecvQ->MoveWritePos(Transferred);

		std::list<std::shared_ptr<StreamBuffer>> _Packet;

		while (true)
		{
			short packetLen = 0;
			_RecvQ->Peek((char *)&packetLen, sizeof(short));

			if (_RecvQ->GetUseSize() < packetLen)
				break;

			std::shared_ptr<StreamBuffer> buffer = StreamBuffer::GetInstance()->get_shared();
			_RecvQ->GetData((char *)buffer.get(), packetLen);

			_Packet.push_back((buffer));
		}

		// Dispatch Packet
		_recvFlag = false;
	}
	RecvPost();
}

bool Session::SendCompletionIO(DWORD Transferred)
{
	{
		std::lock_guard<std::mutex> lock(_sendMutex);

		_SendQ->RemoveData(Transferred);

		_sendFlag = false;
	}

	// Send Noti
}

bool Session::IOCallback(DWORD cbTransferred, LPOVERLAPPED Overlapped)
{
	if (cbTransferred == 0)
	{
		_socket->shutdown(ShutdownBlockMode::BothBlock);
		return true;
	}

	if (Overlapped == nullptr)
		return false;

	if (Overlapped == _sendOverlapped)
		SendCompletionIO(cbTransferred);
	else if (Overlapped == &_recvOverlapped)
		RecvCompletionIO(cbTransferred);

	return true;
}