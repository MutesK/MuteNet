#include "pch.h"
#include "ServerHandleImpl.h"
#include "NetworkManager.h"
#include "Acceptor.h"
#include "SocketFunctionInvoker.h"
#include "SocketUtil.h"

namespace MuteNet
{
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

	bool ServerHandleImpl::Listen(uint16_t Port)
	{
		sockaddr_in name;
		memset(&name, 0, sizeof(name));
		name.sin_family = AF_INET;
		name.sin_port = ntohs(Port);

		_Acceptor = Acceptor::Listen(NetworkManager::Get().GetIOEvent(),
			Callback, nullptr, &name, this);

		if(nullptr == _Acceptor)
			return false;

		return true;
	}

	void ServerHandleImpl::Callback(intptr_t socket, sockaddr* address, int socklen, void* key)
	{
		ServerHandleImpl* Self = reinterpret_cast<ServerHandleImpl*>(key);

		char IpAddress[128];
		uint16_t port;

		switch (address->sa_family)
		{
		case AF_INET:
		{
			sockaddr_in* sock_in = reinterpret_cast<sockaddr_in*>(address);
			MuteNet::inet_ntop(AF_INET, &sock_in->sin_addr, IpAddress, 128);
			port = htons(sock_in->sin_port);
		}
			break;
		case AF_INET6:
			break;
		}

		auto linkCallback = Self->_listenCallbacks->OnInComingConnection(IpAddress, port);

		// Join Socket Ã³¸®
		Self->_errorCode = SocketDelegateInvoker::Invoke(setsockopt, socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
			(const char*)&listen, sizeof(SOCKET));
	}

	}


}
