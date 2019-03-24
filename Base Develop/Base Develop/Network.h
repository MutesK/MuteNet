#pragma once

#include "framework.h"
#include "Session.h"
#include "Acceptor.h"
#include "IOCPManager.h"
#include "Select.h"
#include "SocketAddressFactory.h"
#include "SocketAddress.h"

enum class ServerMechanism
{
	IOCP,
	SELECT
};

class Network : public Singleton<Network>
{
public:
	Network();
	~Network();


	void Init(const std::string& ipAddress, unsigned short port,
		ServerMechanism Mode);
	bool Start();
	void Stop();
	void destroy();
	void run();

	bool Send(LinkHandle link, const std::shared_ptr<StreamBuffer> msg);
	bool SendBroadCast(const std::shared_ptr<StreamBuffer> msg);
	bool Close(LinkHandle link);
private:
	static void OnAccept(LinkHandle link, void *pContext);
	static void OnReceived(LinkHandle link, void *pContext);
	static void OnClosed(LinkHandle link, void *pContext);
	static void OnSended(LinkHandle link, void *pContext);
private:
	std::unique_ptr<Acceptor>		_acceptor;
	std::unique_ptr<IOCPManager>	_iocpIO;

	size_t					_currentSession;
	size_t					_maxSession;

	size_t					_SendPacketCnt;
	size_t					_RecvPacketCnt;
};

