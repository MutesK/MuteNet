#pragma once

#include "Network.h"
#include "NetworkManager.h"
#include "ServerHandleImpl.h"

using namespace MuteNet;

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
		std::cout << "OnRemoteClosed\n";
	}

	virtual void OnTLSHandShakeCompleted() override
	{

	}

	virtual void OnError(int ErrorCode, const std::string& ErrorMsg) override
	{
		std::cout << "Error : " << ErrorCode << " " << ErrorMsg << "["<< __FUNCTION__ << "]" <<  std::endl;

		_link->Shutdown();
	}
};

class ListenCallback : public Network::ListenCallbacks
{
public:
	ListenCallback() = default;

	virtual Link::CallbacksPtr OnInComingConnection(const std::string& RemoteIPAddress, uint16_t RemotePort) override
	{
		printf("InComingConnection (%s:%d)\n", RemoteIPAddress.c_str(), RemotePort);
;
		return std::make_shared<ClientHandle>();
	}

	virtual void OnAccepted(Link& link) override
	{

	}

	virtual void OnError(int ErrorCode, const std::string ErrorMsg) override
	{
	}

};

typedef std::shared_ptr<ListenCallback> ListenCallbackPtr;


class ServerApplication
{
private:
	ListenCallbackPtr		_ListenCallback;
	ServerHandlePtr			_ServerHandlePtr;

public:
	ServerApplication();

	void Listen(const uint16_t Port);
};