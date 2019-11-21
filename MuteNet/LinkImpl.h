#pragma once

#include "Network.h"
#include "Link.h"

namespace MuteNet
{
	class IOContext;

	class LinkImpl;
	typedef std::shared_ptr<LinkImpl> LinkImplPtr;

	class ServerHandleImpl;
	typedef std::shared_ptr<ServerHandleImpl> ServerHandleImplPtr;

	class SessionInfo;

	class LinkImpl final : public Link
	{
		typedef Link super;

		Network::ConnectCallbacksPtr _ConnectCallbacks;

		LinkImplPtr					 _Self;
		ServerHandleImplPtr			 _Server;
		bool						 _isShutdown = false;

		SOCKET						_Socket;

		// 버퍼 관리 고민
		Util::CircularBuffer		_SendBuffer;
		Util::CircularBuffer		_RecvBuffer;

		std::string					 _LocalIP;
		uint16_t					 _LocalPort;
		std::string					 _RemoteIP;
		uint16_t					 _RemotePort;
	public:
		LinkImpl(const CallbacksPtr LinkCallback);
		LinkImpl(intptr_t socket, const CallbacksPtr LinkCallback,
			const ServerHandleImplPtr ServerHandlePtr, const sockaddr* Addr,
			size_t socketLen);
		~LinkImpl() override;

		static LinkImplPtr Connect(std::string& Host, uint16_t Port,
			Link::CallbacksPtr LinkCallbacks, Network::ConnectCallbacksPtr ConnectCallbacks);

		void Enable(LinkImplPtr Self);
		bool Send(const void* Data, size_t Length) override;

		std::string GetLocalIP() const override;
		uint16_t GetLocalPort() const override;
		std::string GetRemoteIP() const override;
		uint16_t GetRemotePort() const override;

		CallbacksPtr GetCallbacks();

		void Shutdown() override;
		void Close() override;
	private:
		friend class Network;
		friend class SocketUtil;
		friend class ASyncSendRequest;
		friend class ASyncRecvRequest;

	};

	inline std::string LinkImpl::GetLocalIP() const
	{
		return _LocalIP;
	}

	inline uint16_t LinkImpl::GetLocalPort() const
	{
		return _LocalPort;
	}

	inline std::string LinkImpl::GetRemoteIP() const
	{
		return _RemoteIP;
	}

	inline uint16_t LinkImpl::GetRemotePort() const
	{
		return _RemotePort;
	}
	inline Link::CallbacksPtr LinkImpl::GetCallbacks()
	{
		return _Callback;
	}
}
