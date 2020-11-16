#include "Common.h"
#include "TypeDefine.hpp"
#include "IPLookUp.h"
#include <TaskManager.h>

namespace MuteNet
{
	using IPLookupPtr = std::shared_ptr<IPLookUp>;
	using IPLookupPtrs = std::vector<IPLookupPtr>;
	
	IPLookUp::IPLookUp ( const std::string &IP, Network::ResolveDomainNameCallbacksPtr Callback )
			: _Callback ( Callback ), _IP ( IP )
	{
	}
	
	void IPLookUp::Lookup ( const std::string &IP, Network::ResolveDomainNameCallbacksPtr Callback )
	{
		IPLookupPtr Lookup {new IPLookUp ( IP, std::move ( Callback ))};
		
		auto ASyncTask = [ Lookup ] ( )
		{
			sockaddr_storage address_storage;
			
			int salen = sizeof ( address_storage );
			memset ( &address_storage, 0, sizeof ( address_storage ));
			
			int err = SocketUtil::ParseSockaddrPort ( Lookup->_IP,
			                                          reinterpret_cast<sockaddr *>(&address_storage), &salen );
			
			if ( err != 0 )
			{
				LogHelper::Log ( ELogLevel::Debug, "Failed to Parse IP Address {0}.", Lookup->_IP );
				Lookup->Callback ( err, std::string ( ));
			}
			
			std::string HostName;
			std::string ServInfo;
			
			err = SocketUtil::GetDomainNameInfo ( reinterpret_cast<sockaddr *>(&address_storage),
			                                      salen, HostName, ServInfo );
			
			Lookup->Callback ( err, HostName );
			
		};
		
		Enqueue_TASK( ASyncTask );
		
	}
	
	void IPLookUp::Callback ( int Result, const std::string Address )
	{
		if ((Result != 0) || (Address.length ( ) == 0))
		{
			// An error has occurred, notify the error callback:
			_Callback->OnError ( Result, SocketUtil::ErrorString ( Result ));
		} else
		{
			// Call the success handler:
			_Callback->OnNameResolved ( Address, _IP );
			_Callback->OnFinished ( );
		}
	}
	
}
