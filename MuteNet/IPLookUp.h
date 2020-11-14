#pragma once

#include "Network.h"

// Ip�� ���� hostname(domain)�� ã�´�.

namespace MuteNet
{
	class IPLookUp
	{
	protected:
		Network::ResolveDomainNameCallbacksPtr _Callback;
		
		std::string _IP;
	public:
		static void Lookup ( const std::string &IP, Network::ResolveDomainNameCallbacksPtr Callback );
	
	private:
		IPLookUp ( const std::string &IP, Network::ResolveDomainNameCallbacksPtr Callback );
		
		void Callback ( int Result, const std::string Address );
	};
}

