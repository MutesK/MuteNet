#pragma once

namespace MuteNet
{
	class ServiceListener;
	struct ExtensionFunctions;

	typedef void (*listener_callback)(intptr_t, struct sockaddr*, int socklen, void*);


	class Acceptor final
	{
		ServiceListener*			_port;
		SOCKET						_listen;
		ExtensionFunctions*			_extension;
		listener_callback			_callback;
	public:
		Acceptor(const ServiceListener* Port, const listener_callback& Callback);
		bool Listen(int Family, int Backlog, uint8_t Port);

	};
}

