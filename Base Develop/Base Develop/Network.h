#pragma once

#include "framework.h"
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
	void Start(NetworkMode networkMode,
		std::string resolveDNSAddress, size_t maxSession);
	void Stop();

public:
	// Network IO Request For User Contents
	void SendPacket(const LinkHandle& handle,
		const std::shared_ptr<StreamBuffer> buffer);

	bool CloseHandle(const LinkHandle& handle);

	bool GetNetworkInfo(const LinkHandle& handle,
		std::pair<std::string, int>& OUT networkinfo);
public:
	// Override IO Notify To User Contents
	virtual void OnConnected(const LinkHandle clientHandle) = 0;
	virtual void OnDisconnected(const LinkHandle clientHandle) = 0;
	virtual void OnRecvied(const LinkHandle clientHandle,
		const std::shared_ptr<StreamBuffer> buffer) = 0;
private:
	void initIOCPMoudle(NetworkMode& Mode, 
		std::shared_ptr<SocketAddress>& address);

private:
	// Network Callback IO INNER Process 
	void OnAccept(TcpSocketPtr socket);
private:
	std::unique_ptr<Acceptor>		_acceptor;
	std::unique_ptr<IOCPManager>	_iocpIO;

	WSADATA					_winsockData;

	size_t					_currentSession;
	size_t					_maxSession;
};

