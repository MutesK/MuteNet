#pragma once
#include "SocketIO.h"

/*
	리팩토링 필수.
*/
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
		friend class ServiceListener;
		friend class ASyncAcceptRequest;
	private:
		ServiceListener*			_Port;
		SOCKET						_Listen;
		SOCKADDR_IN*				_Address;
		int							_Backlog;
		ListenerCallback			_Callback;
		ListenerErrorCallback		_ErrorCallback;
		void*						_Key;
		AcceptorPtr					_Self;

		bool						_StopTrigger = false;
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

		void StartAccept() const;
	};

}

