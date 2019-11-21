#include "ServerApplication.h"


class ListenCallback : public Network::ListenCallbacks
{
private:
	ServerApplication& _App;
public:
	ListenCallback(ServerApplication& App)
		:_App(App)
	{
	}

	virtual Link::CallbacksPtr OnInComingConnection(const std::string& RemoteIPAddress,
		uint16_t RemotePort) override
	{

	}

	virtual void OnAccepted(Link& link) override
	{

	}

	virtual void OnError(int ErrorCode, const std::string ErrorMsg) override
	{

	}

};

ServerApplication::ServerApplication()
{
	ServerHandleImpl::Listen(25000, );
}
