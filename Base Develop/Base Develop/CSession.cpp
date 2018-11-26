#include "stdafx.h"
#include "CSession.h"
#include "SocketUtil.h"
#include "TCPSocket.h"
#include "UDPSocket.h"
#include "SocketAddress.h"

CSession::CSession(std::shared_ptr<TCPSocket>& socket,
	std::shared_ptr<SocketAddress>& address)
{
	_tcpSocket = socket;
	_address = address;
}


CSession::~CSession()
{
}
