#pragma once

#include "Network.h"

// Ip를 통해 hostname(domain)을 찾는다.

namespace MuteNet
{
	class IPLookUp
	{
	protected:
		Network::ResolveDomainNameCallbacksPtr _Callback;
		
		std::string _IP;
	public:
		static void Lookup(const std::string& IP, Network::ResolveDomainNameCallbacksPtr Callback);

	private:
		IPLookUp(const std::string& IP, Network::ResolveDomainNameCallbacksPtr Callback);

		void Callback(int Result, const std::string Address);
	};
}

