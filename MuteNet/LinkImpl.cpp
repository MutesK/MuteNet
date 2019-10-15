#include "pch.h"
#include "LinkImpl.h"
#include "NetworkManager.h"

namespace MuteNet
{
	LinkImpl::LinkImpl(const CallbacksPtr LinkCallback)
		:super(LinkCallback)
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
				// using Connector
			}

			void OnError(int ErrorCode, const std::string& ErrorMsg) override
			{
				_link->GetCallbacks()->OnError(ErrorCode, ErrorMsg);
				NetworkManager::Get().RemoveLink(_link.get());
			}

			void OnNameResolved(const std::string& Name, const std::string& IP) override
			{
				// Not to Work
			}

			void OnFinished(void)
			{

			}
		};

		Network::HostnameToIP(Host, 
			std::make_shared<DomainCallbacks>(link, Port));

		return link;
	}

}