#pragma once
#include "SocketIO.h"

namespace MuteNet
{
	class ServiceListener;
	struct ExtensionFunctions;

	class Acceptor;
	typedef std::shared_ptr<Acceptor> AcceptorPtr;

	typedef void (*ListenerCallback)(intptr_t s, struct sockaddr* address, int socklen, void*);
	typedef void (*ListenerErrorCallback)(intptr_t s, int errorCode, std::string errorString);

	class Acceptor
	{
		enum SocketWorkType
		{
			SocketPool = 0,
			Fixed = 1,
		};

		struct AcceptorTask
		{
			Overlapped		_AcceptOverlapped;
			Acceptor*		_acceptor;
			SOCKET			_socket;
			char			_addrbuf[30];
			int				_buflen;

			AcceptorTask(Acceptor* acceptor, ServiceListener* Port)
			{
				_acceptor = acceptor;
				_buflen = 30;
				_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			}
		};

		friend class ServiceListener;

	private:
		ServiceListener*			_port;
		SOCKET						_listen;
		SOCKADDR_IN*				_address;
		int							_backlog;
		ListenerCallback			_callback;
		ListenerErrorCallback		_errorcallback;
		void*						_key;

		bool						_stopTrigger = false;
	public:
		static AcceptorPtr Listen(ServiceListener* Port, 
			ListenerCallback Callback, ListenerErrorCallback ErrorCallback,
			SOCKADDR_IN* Ip, void* AdditionalKey, int Backlog = 0);

		void Start();
		void Stop();
	private:
		Acceptor(ServiceListener* Port, ListenerCallback& Callback, ListenerErrorCallback& ErrorCallback,
			SOCKADDR_IN* Ip, void* key, int Backlog);

		void InitializeListenSocket();

		void StartAccept();
		static void OnAccepted(Overlapped* pOverlapped, uintptr_t socket, 
			int transferredBytes, int Success);
	};

}

