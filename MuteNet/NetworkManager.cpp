#include "pch.h"
#include "NetworkManager.h"
#include "Link.h"
#include "ServerHandle.h"

namespace MuteNet
{
	NetworkManager::NetworkManager()
	{
		Initialize();
	}

	NetworkManager::~NetworkManager()
	{
		WSACleanup();
	}

	void NetworkManager::Initialize()
	{
		WSADATA wsaData;

		auto res = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if(res != 0)
		{
			int err = WSAGetLastError();
			exit(1);
		}

		_IOEvent = ServiceListenerPtr{ new ServiceListener() };
		_IOEvent->Initialize(std::thread::hardware_concurrency(), INFINITE);
	}

	void NetworkManager::Terminate()
	{
		SAFE_UNIQUELOCK(_Lock);

		auto Connections = _Links;
		for(auto& Conn : Connections)
		{
			Conn->Close();
		}

		auto Servers = _Servers;
		for(auto& Server : Servers)
		{
			Server->Close();
		}
	}

	void NetworkManager::AddLink(LinkPtr link)
	{
		SAFE_UNIQUELOCK(_Lock);

		_Links.push_back(link);
	}

	void NetworkManager::RemoveLink(const Link* link)
	{
		SAFE_SHAREDLOCK(_Lock);

		for(auto iter = _Links.begin(); iter != _Links.end(); ++iter)
		{
			if(iter->get() == link)
			{
				_Links.erase(iter);
				return;
			}
		}
	}

	void NetworkManager::AddServer(ServerHandlePtr Server)
	{
		SAFE_UNIQUELOCK(_Lock);

		_Servers.push_back(Server);
	}

	void NetworkManager::RemoveServer(const ServerHandle* Server)
	{
		SAFE_SHAREDLOCK(_Lock);

		for (auto iter = _Servers.begin(); iter != _Servers.end(); ++iter)
		{
			if (iter->get() == Server)
			{
				_Servers.erase(iter);
				return;
			}
		}
	}
	ServiceListener* NetworkManager::GetIOEvent() const
	{
		return _IOEvent.get();
	}
}
