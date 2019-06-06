#pragma once

#include "framework.h"
#include "Session.h"
#include "Acceptor.h"
#include "IOCPManager.h"
#include "Select.h"
#include "SocketAddressFactory.h"
#include "SocketAddress.h"
#include "StreamBuffer.h"

class Network : public Singleton<Network>
{
public:
	Network();
	~Network();

	void Init(const std::string& address, size_t maxSession);
	bool Start();

	// 1. 신규 클라이언트 접근 막음.
	// 2. Send Packet 중단
	void Stop();
	void Destroy();

	bool Send(LinkHandle link, const std::shared_ptr<StreamBuffer> msg);
	bool SendBroadCast(const std::shared_ptr<StreamBuffer> msg);
	bool Close(LinkHandle link);
private:
	static void OnAccept(LinkHandle link);
	static void OnReceived(LinkHandle link, void *pContext);
	static void OnClosed(LinkHandle link);
	static void OnSended(LinkHandle link, int SendSize);

private:
	std::unique_ptr<Acceptor>		_acceptor;
	std::shared_ptr<IOCPManager>	_socketio;

	static size_t					_currentSession;
	static size_t					_maxSession;

	static size_t					_SendPacketCnt;
	static size_t					_RecvPacketCnt;

	friend class Session;
	friend class Acceptor;
};

