#pragma once

#include "Network.h"
#include "NetworkManager.h"
#include "ServerHandleImpl.h"
#include "MiniDump.h"
#include "LogHelper.h"

using namespace MuteNet;
using namespace Util;

class ClientHandle : public Link::Callbacks
{
private:
	Link* _link = nullptr;
public:
	ClientHandle() = default;
	virtual ~ClientHandle() = default;

	virtual void OnCreated(Link* Link) override
	{
		_link = Link;
	}

	virtual void OnReceivedData(const char* data, size_t Length) override
	{
		_link->Send(data, Length);
	}

	virtual void OnRemoteClosed(void) override
	{
		LogHelper::Log(ELogLevel::Debug, "OnRemoteClosed");
	}

	virtual void OnTLSHandShakeCompleted() override
	{

	}

	virtual void OnError(int ErrorCode, const std::string& ErrorMsg) override
	{
		LogHelper::Log(ELogLevel::Fatal, "Error {0} {1} [{2}]", ErrorCode, ErrorMsg, __FUNCTION__);

		_link->Shutdown();
	}
};

class ListenCallback : public Network::ListenCallbacks
{
public:
	ListenCallback() = default;

	virtual Link::CallbacksPtr OnInComingConnection(const std::string& RemoteIPAddress, uint16_t RemotePort) override
	{		
		LogHelper::Log(ELogLevel::Debug, "InComingConnection({0}:{1})", RemoteIPAddress, RemotePort);

		return std::make_shared<ClientHandle>();
	}

	virtual void OnAccepted(Link& link) override
	{

	}

	virtual void OnError(int ErrorCode, const std::string ErrorMsg) override
	{
		throw new std::exception(ErrorMsg.c_str());
	}

};

typedef std::shared_ptr<ListenCallback> ListenCallbackPtr;

class ServerApplication
{
private:
	ListenCallbackPtr		_ListenCallback;
	ServerHandlePtr			_ServerHandlePtr;

	Util::MiniDump			_Dump;
public:
	ServerApplication();

	void Listen(const uint16_t Port);
};