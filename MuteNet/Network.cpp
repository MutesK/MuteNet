#include "pch.h"
#include "Network.h"
#include "LinkImpl.h"
#include "NetworkManager.h"
#include "HostNameLookup.h"
#include "IPLookup.h"
#include "ServerHandleImpl.h"
#include "LinkImpl.h"


namespace MuteNet
{
	LinkImplPtr Network::Connect ( const std::string &host, uint16_t port, ConnectCallbacksPtr connectCallbacks,
	                               Link::CallbacksPtr LinkCallbacks )
	{
		LinkImplPtr link {new LinkImpl ( LinkCallbacks )};
		link->_ConnectCallbacks = connectCallbacks;
		NetworkManager::Get ( ).AddLink ( link );
		LinkCallbacks->OnCreated ( link.get ( ));
		
		class HostnameCallback : public Network::ResolveDomainNameCallbacks
		{
			LinkImplPtr _Link;
			uint16_t _port;
			bool _isConnect;
		
		public:
			HostnameCallback ( LinkImplPtr Link, uint16_t Port )
					: _Link ( std::move ( Link )), _port ( Port ), _isConnect ( false )
			{
			}
			
			void Connect ( const sockaddr *IP, int Size )
			{
				if ( !_isConnect )
				{
				
				}
			}
			
			
			virtual bool OnNameResolvedIPv4 ( const std::string &Name, const sockaddr_in *Ip ) override
			{
				sockaddr_in Addr = *Ip;
				Addr.sin_port = htons ( _port );
				Connect ( reinterpret_cast<const sockaddr *>(&Addr), sizeof ( Addr ));
				
				return false;
			}
			
			virtual bool OnNameResolvedIPv6 ( const std::string &Name, const sockaddr_in6 *Ip ) override
			{
				sockaddr_in6 Addr = *Ip;
				Addr.sin6_port = htons ( _port );
				Connect ( reinterpret_cast<const sockaddr *>(&Addr), sizeof ( Addr ));
				
				return false;
			}
			
			virtual void OnError ( int ErrorCode, const std::string &ErrorMsg ) override
			{
				_Link->GetCallbacks ( )->OnError ( ErrorCode, ErrorMsg );
				
				NetworkManager::Get ( ).RemoveLink ( _Link.get ( ));
			}
			
			virtual void OnFinished ( ) override
			{
			}
			
			virtual void OnNameResolved ( const std::string &Name, const std::string &IP ) override
			{
			}
		};
		
		Network::HostnameToIP ( host, std::make_shared<HostnameCallback> ( link, port ));
		return link;
	}
	
	ServerHandlePtr Network::Listen ( uint16_t Port, ListenCallbacksPtr listenCallbacks )
	{
		return ServerHandleImpl::Listen ( Port, listenCallbacks );
	}
	
	bool Network::HostnameToIP ( const std::string &hostName, ResolveDomainNameCallbacksPtr callbacks )
	{
		HostNameLookup::Lookup ( hostName, std::move ( callbacks ));
		return true;
	}
	
	bool Network::IPToHostName ( const std::string &Ip, ResolveDomainNameCallbacksPtr callbacks )
	{
		IPLookUp::Lookup ( Ip, std::move ( callbacks ));
		return true;
		
	}
}


