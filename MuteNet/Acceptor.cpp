#include "pch.h"
#include "Acceptor.h"
#include "ServiceListener.h"
#include "SocketUtil.h"

namespace MuteNet
{
	Acceptor::Acceptor(const ServiceListener* Port, const listener_callback& Callback)
		:_port(const_cast<ServiceListener*>(Port)), _listen(INVALID_SOCKET), _callback(Callback)
	{
		_extension = _port->GetExtension();
	}

	bool Acceptor::Listen(int family, int backlog, uint8_t Port)
	{
		if (_extension == nullptr || _port == nullptr)
			return false;

		if (_extension->_AcceptEx == nullptr || _extension->_GetAcceptExSockaddrs == nullptr)
			return false;

		_listen = socket(family, SOCK_STREAM, IPPROTO_TCP);

		if (_listen == INVALID_SOCKET)
			return false;

		bool on = true;
		if (!SocketDelegateInvoker::Invoke(setsockopt, _listen, SOL_SOCKET, SO_CONDITIONAL_ACCEPT,
			reinterpret_cast<char*>(&on), sizeof(on)))
			return false;

		if (!listen_socket_reuseable(_listen))
			return false;

		{
			sockaddr_in name;
			ZeroMemory(&name, sizeof(name));
			name.sin_family = AF_INET;
			name.sin_port = ntohs(Port);

			if (!SocketDelegateInvoker::Invoke(bind, reinterpret_cast<const sockaddr*>(&name), sizeof(name)))
				return false;
		}

		if (!make_socket_nonblocking(_listen))
			return false;

		if (listen(_listen, backlog) != 0)
			return false;



		return true;
	}
}
