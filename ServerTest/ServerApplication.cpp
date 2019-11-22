#include "ServerApplication.h"

ServerApplication::ServerApplication()
	:_ListenCallback(std::make_shared<ListenCallback>())
{
}

void ServerApplication::Listen(const uint16_t Port)
{
	_ServerHandlePtr = Network::Listen(Port, _ListenCallback);
}
