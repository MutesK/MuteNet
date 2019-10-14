#pragma once

namespace MuteNet
{
	class ServiceListener;
	struct ExtensionFunctions;

	class Acceptor;
	typedef std::shared_ptr<Acceptor> AcceptorPtr;

	typedef void (*ListenerCallback)(intptr_t s, struct sockaddr* address, int socklen, void*);
	typedef void (*ListenerErrorCallback)(intptr_t s, int errorCode, std::string errorString);

	struct AcceptorTask
	{
		OVERLAPPED		_overlapped;
		Acceptor*		_acceptor;
		SOCKET			_socket;
		char			_addrbuf[1];
		int				_buflen;
	};

	class Acceptor
	{
		ServiceListener*			_port;
		SOCKET						_listen;
		SOCKADDR_IN*				_address;
		int							_backlog;
		ListenerCallback			_callback;
		ListenerErrorCallback		_errorcallback;
		void*						_key;
	public:
		static AcceptorPtr Listen(ServiceListener* Port, 
			ListenerCallback Callback, ListenerErrorCallback ErrorCallback,
			SOCKADDR_IN* Ip, void* AdditionalKey, int Backlog = 0);

	private:
		Acceptor(ServiceListener* Port, ListenerCallback& Callback, ListenerErrorCallback& ErrorCallback,
			SOCKADDR_IN* Ip, void* key, int Backlog);

		void InitializeListenSocket();
	};

}

