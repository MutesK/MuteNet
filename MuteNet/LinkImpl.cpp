#include "pch.h"
#include "LinkImpl.h"

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
		class DomainCallback :
			public Network::ResolveDomainNameCallbacks
		{
			void OnNameResolved(const std::string& Name, const std::string& IP) override
			{

			}

			bool OnNameResolvedIPv4(const std::string& Name, const sockaddr_in* Ip) override
			{

			}

			bool OnNameResolvedIPv6(const std::string& Name, const sockaddr_in6* Ip) override
			{

			}
		};
	}

}