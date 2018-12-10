#include "stdafx.h"
#include "CSession.h"
#include "SocketUtil.h"
#include "TCPSocket.h"
#include "UDPSocket.h"
#include "SocketAddress.h"
#include "MemoryStream.h"
#include "InputMemoryStream.h"
#include "OutputMemoryStream.h"

CSession::CSession(std::shared_ptr<TCPSocket>& socket,
	std::shared_ptr<SocketAddress>& address)
{
	_tcpSocket = socket;
	_address = address;

	_OutputStreamBuffer = std::make_shared<OutputMemoryStream>();
	_InputStreamBuffer = std::make_shared<InputMemoryStream>();
}


CSession::~CSession()
{
}

bool CSession::Recv()
{
	auto pStream = std::dynamic_pointer_cast<InputMemoryStream>(_InputStreamBuffer);
}