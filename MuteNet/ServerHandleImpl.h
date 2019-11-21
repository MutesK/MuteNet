#pragma once

#include "pch.h"
#include "ServerHandle.h"
#include "Network.h"

namespace MuteNet
{
	class ServerHandleImpl;
	typedef std::shared_ptr<ServerHandleImpl> ServerHandleImplPtr;
	class Acceptor;
	typedef std::shared_ptr<Acceptor> AcceptorPtr;

	class ServerHandleImpl : public ServerHandle
	{
		typedef ServerHandle super;
		friend class LinkImpl;
	protected:
		Network::ListenCallbacksPtr _listenCallbacks;
		AcceptorPtr					_Acceptor;
		bool						_isListening;
		LinkImplPtrs				_Connections;
		int							_errorCode;
		std::string					_errorMsg;
		ServerHandleImplPtr			_selfPtr;

		// Acceptor
	public :
		virtual ~ServerHandleImpl() override;

		static ServerHandleImplPtr Listen(uint16_t Port,
			Network::ListenCallbacksPtr& ListenCallback);

		void Close() override;
		
		bool IsListening() const override
		{
			return _isListening;
		}

	protected:
		ServerHandleImpl(Network::ListenCallbacksPtr ListenCallback);

		bool Listen(uint16_t Port);

		void RemoveLink(const LinkImpl* Link);

		static void Callback(intptr_t socket, struct sockaddr* address,
			int socklen, void* key);

		static void ErrorCallback(intptr_t socket, int errorCode, std::string errorString);
	};
}

