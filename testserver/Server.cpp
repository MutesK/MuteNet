#include "Common.h"
#include "ServerHandleImpl.hpp"
#include "IoContextEvent.hpp"
#include <memory>

using namespace MuteNet;
using namespace EventLoop;


class UserCallback : TCPLink::Callbacks
{
	TCPLink* _self;

	// Callbacks을(를) 통해 상속됨
	virtual void OnCreated(TCPLink* Link) override
	{
		_self = Link;
	}

	virtual void OnReceivedData(const char* data, size_t Length) override
	{

	}

	virtual void OnRemoteClosed(void) override
	{

	}

	virtual void OnTLSHandShakeCompleted() override
	{

	}

	virtual void OnError(int ErrorCode, const std::string& ErrorMsg) override
	{

	}
};


class ListenCallback : public NetworkHelpers::ListenCallbacks
{
	virtual TCPLink::CallbacksPtr OnInComingConnection(const std::string& RemoteIPAddress,
		uint16_t RemotePort) override
	{
		std::cout << "Incoming IP : " << RemoteIPAddress << ":" << RemotePort << std::endl;
		return std::reinterpret_pointer_cast<TCPLink::Callbacks>(std::make_shared<UserCallback>());
	}

	virtual void OnAccepted(TCPLink& link) override
	{
		std::cout << "Accepted Client : " << link.GetLocalIP() << std::endl;
	}

	virtual void OnError(int ErrorCode, std::string ErrorMsg) override
	{
		std::cout << ErrorMsg << std::endl;
	}
};

int main()
{
	IOContextEvent Event(4, INFINITE);
	NetworkHelpers::ListenCallbacksPtr listenCallback = std::make_shared<ListenCallback>();

	auto handle = ServerHandleImpl::Listen(Event, 1225u, listenCallback);

	while (true)
	{
		// main loop thred
	}
}