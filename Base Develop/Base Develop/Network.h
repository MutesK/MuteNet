#pragma once

#include "../Foundation/foundation.h"
#include "Session.h"
#include "Acceptor.h"
#include "IOCPManager.h"
#include "Select.h"
#include "SocketAddressFactory.h"
#include "SocketAddress.h"

enum NetworkMode
{
	Server,
	Client,
};

class Network
{
public:
	Network();
	~Network();

	// IO를 시작한다.
	void Start(NetworkMode networkMode, SocketAddress& address);
	void Stop();

	void OnAccept(TcpSocketPtr ptr);
private:
	void initIOCPMoudle(NetworkMode& Mode, SocketAddress& address);
private:
	/// Server Mode : Listen, Client : socket
	TcpSocketPtr					_socket; 
	std::unique_ptr<Acceptor>		_acceptor;
	std::unique_ptr<IOCPManager>	_iocpIO;

	SessionManager			  _sessionManager;
};

