#pragma once

namespace MuteNet
{
	class ServiceListener;
	struct ExtensionFunctions;

	class Acceptor;
	typedef std::shared_ptr<Acceptor> AcceptorPtr;

	typedef void (*listener_callback)(intptr_t s, struct sockaddr* address, int socklen, void*);
	typedef void (*listener_errorcallback)(intptr_t s, int errorCode, std::string errorString);

	struct AcceptorTask
	{
		SOCKET			_socket;
		OVERLAPPED		_overlapped;
		char			_addrbuf[1];
		int				_buflen;
	};

	class Acceptor
	{
		ServiceListener*			_port;
		SOCKET						_listen;
		SOCKADDR_IN*				_address;
		int							_backlog;
		listener_callback			_callback;
		listener_errorcallback		_errorcallback;
	public:
		AcceptorPtr Listen(ServiceListener* Port, 
			listener_callback& Callback, listener_errorcallback& ErrorCallback, 
			SOCKADDR_IN* Ip, int Backlog = 0);

	private:
		Acceptor(ServiceListener* Port, listener_callback& Callback, listener_errorcallback& ErrorCallback,
			SOCKADDR_IN* Ip, int Backlog);

		void InitializeListenSocket();
	};

}

