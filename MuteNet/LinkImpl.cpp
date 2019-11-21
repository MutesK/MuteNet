#include "pch.h"
#include "LinkImpl.h"
#include "NetworkManager.h"
#include "Session.h"
#include "SocketUtil.h"
#include "ServerHandleImpl.h"
#include "ASyncSendRequest.h"

namespace MuteNet
{
	LinkImpl::LinkImpl(const CallbacksPtr LinkCallback)
		:super(LinkCallback)
		,_LocalPort(0)
		,_RemotePort(0)
		,_isShutdown(false)
	{
	}

	LinkImpl::LinkImpl(intptr_t socket, const CallbacksPtr LinkCallback, const ServerHandleImplPtr ServerHandlePtr, const sockaddr* Addr, size_t socketLen)
		:super(LinkCallback)
		, _Server(ServerHandlePtr)
		, _LocalPort(0)
		, _RemotePort(0)
		, _isShutdown(false)
		, _Socket(socket)
	{
	}

	LinkImpl::~LinkImpl()
	{
		
	}

	LinkImplPtr LinkImpl::Connect(std::string& Host, uint16_t Port, Link::CallbacksPtr LinkCallbacks,
		Network::ConnectCallbacksPtr ConnectCallbacks)
	{
		LinkImplPtr link { new LinkImpl(LinkCallbacks) };
		link->_ConnectCallbacks = ConnectCallbacks;

		// 임시 Callback class 정의
		class DomainCallbacks :
			public Network::ResolveDomainNameCallbacks
		{
			LinkImplPtr _link;
			uint16_t	_port;
			bool		_isConnecting;
		public:
			DomainCallbacks(LinkImplPtr Link, uint16_t Port)
				:_link(std::move(Link)), _port(Port), _isConnecting(false)
			{
			}

			bool OnNameResolvedIPv4(const std::string& Name, const sockaddr_in* Ip) override
			{
				sockaddr_in addr = *Ip;
				addr.sin_port = htons(_port);
				
				Connect(reinterpret_cast<const sockaddr*>(&addr), sizeof(addr));
				return false;
			}

			bool OnNameResolvedIPv6(const std::string& Name, const sockaddr_in6* Ip) override
			{
				sockaddr_in6 addr = *Ip;
				addr.sin6_port = htons(_port);

				Connect(reinterpret_cast<const sockaddr*>(&addr), sizeof(addr));
				return false;
			}

			void Connect(const sockaddr* Ip, int size)
			{
				if (!_isConnecting)
				{
					int ErrCode = SocketUtil::Connect(_link, Ip, size);
					if (ErrCode == 0)
					{
						_isConnecting = true;
					}
					else
					{
						_link->GetCallbacks()->OnError(ErrCode, SocketUtil::ErrorString(ErrCode));
					}
				}
			}

			void OnError(int ErrorCode, const std::string& ErrorMsg) override
			{
				_link->GetCallbacks()->OnError(ErrorCode, ErrorMsg);
				NetworkManager::Get().RemoveLink(_link.get());
			}

			void OnNameResolved(const std::string& Name, const std::string& IP) override
			{
			}

			void OnFinished(void)
			{
			}
		};

		Network::HostnameToIP(Host, 
			std::make_shared<DomainCallbacks>(link, Port));

		return link;
	}

	// Iocp Recv, Send, Event Callback
	void LinkImpl::Enable(LinkImplPtr Self)
	{
		_Self = Self;

		const auto Event = NetworkManager::Get().GetIOEvent();
		Event->RegisterHandle(reinterpret_cast<void *>(_Socket), nullptr);


	}

	// SendASyncRequest Idea 적용
	bool LinkImpl::Send(const void* Data, size_t Length)
	{
		if (_isShutdown || _SendBuffer.get_isSending())
		{
			return false;
		}

		auto SendRequest = std::make_shared<ASyncSendRequest>(_Self, Data, Length);

		SendRequest->Process();
	}

	void LinkImpl::Shutdown()
	{
		::shutdown(_Socket, SD_SEND);
	}
	
	void LinkImpl::Close()
	{
		::closesocket(_Socket);

		if (_Server == nullptr)
		{
			NetworkManager::Get().RemoveLink(this);
		}
		else
		{
			_Server->RemoveLink(this);
		}

		_Self.reset();
	}
}