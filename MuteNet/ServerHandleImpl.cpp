#include "pch.h"
#include "ServerHandleImpl.h"
#include "NetworkManager.h"
#include "Acceptor.h"
#include "SocketUtil.h"
#include "LinkImpl.h"

namespace MuteNet
{
	ServerHandleImpl::~ServerHandleImpl()
	{
	}

	ServerHandleImpl::ServerHandleImpl(Network::ListenCallbacksPtr ListenCallback)
		:_listenCallbacks(ListenCallback),
		_Acceptor(nullptr), _isListening(false), _errorCode(0)
	{
	}

	ServerHandleImplPtr ServerHandleImpl::Listen(uint16_t Port, Network::ListenCallbacksPtr& ListenCallbacks)
	{
		ServerHandleImplPtr res = ServerHandleImplPtr
		{ new ServerHandleImpl(ListenCallbacks) };

		res->_selfPtr = res;
		if (res->Listen(Port))
		{
			NetworkManager::Get().AddServer(res);
		}
		else
		{
			res->_listenCallbacks->OnError(res->_errorCode, res->_errorMsg);
			res->_selfPtr.reset();
			return nullptr;
		}
	}

	void ServerHandleImpl::Close()
	{
		_Acceptor->Stop();
	}

	bool ServerHandleImpl::Listen(uint16_t Port)
	{
		sockaddr_in name;
		memset(&name, 0, sizeof(name));
		name.sin_family = AF_INET;
		name.sin_port = ntohs(Port);

		_Acceptor = Acceptor::Listen(NetworkManager::Get().GetIOEvent(),
			Callback, nullptr, &name, this);

		if (nullptr == _Acceptor)
			return false;

		_isListening = true;
		return true;
	}

	void ServerHandleImpl::RemoveLink(const LinkImpl* Link)  // 개선필요
	{
		std::for_each(_Connections.begin(), _Connections.end(), [&](const auto& Iter)
			{
				if (Iter->get() == Link)
				{
					_Connections.erase(itr);
					return;
				}
			});
	}

	void ServerHandleImpl::Callback(intptr_t socket, sockaddr* address, int socklen, void* key)
	{
		ServerHandleImpl* Self = reinterpret_cast<ServerHandleImpl *>(key);

		char IpAddress[128]{0};
		uint16_t port = 0;

		switch (address->sa_family)
		{
		case AF_INET:
		{
			sockaddr_in* sock_in = reinterpret_cast<sockaddr_in*>(address);
			SocketUtil::inet_ntop(AF_INET, &sock_in->sin_addr, IpAddress, 128);
			port = htons(sock_in->sin_port);
		}
		break;
		case AF_INET6:
		{
			sockaddr_in6* sock_in6 = reinterpret_cast<sockaddr_in6*>(address);
			SocketUtil::inet_ntop(AF_INET6, &sock_in6->sin6_addr, IpAddress, 128);
			port = htons(sock_in6->sin6_port);
		}
		break;
		}

		auto LinkCallbacks = Self->_listenCallbacks->OnInComingConnection(IpAddress, port);
		if (LinkCallbacks == nullptr)
		{
			closesocket(socket);
			return;
		}

		LinkImplPtr Link = std::make_shared<LinkImpl>(socket, LinkCallbacks,
			Self->_selfPtr, address, socklen);
		
		{
			Self->_Connections.emplace_back(Link);
		}

		LinkCallbacks->OnCreated(Link.get());
		Link->Enable(Link);
		Self->_listenCallbacks->OnAccepted(*Link);

	}
}
