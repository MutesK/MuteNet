#include "pch.h"
#include "ServerHandleImpl.h"
#include "NetworkManager.h"
#include "Acceptor.h"

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


}
