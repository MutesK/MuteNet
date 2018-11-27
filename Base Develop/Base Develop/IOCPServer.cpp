#include "stdafx.h"
#include "IOCPServer.h"
#include "Acceptor.h"
#include "CSession.h"
#include "TCPSocket.h"

IOCPServer::IOCPServer()
{
	_iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);

	_acceptor = std::make_shared<Acceptor>("tcp://127.0.0.1", &OnAccept);
}


IOCPServer::~IOCPServer()
{
}


void IOCPServer::OnAccept(std::shared_ptr<CSession>& Session)
{
	auto Socket = Session->get_tcpSocket()->get_sock();

	if (Socket == INVALID_SOCKET)
		return;

	CreateIoCompletionPort(reinterpret_cast<HANDLE>(Socket), _iocpHandle,
		reinterpret_cast<ULONG>(Session.get()), 0);


}