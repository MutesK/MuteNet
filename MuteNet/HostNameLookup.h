#pragma once

#include "Network.h"

namespace MuteNet
{
    class HostNameLookup
    {
    public:
        static void Lookup(const std::string &hostName, Network::ResolveDomainNameCallbacksPtr Callbacks);

    protected:
        Network::ResolveDomainNameCallbacksPtr _Callbacks;

        std::string _hostName;
    protected:
        HostNameLookup(const std::string &hostName, Network::ResolveDomainNameCallbacksPtr Callbacks);

        void PreProcessCallback(int ErrorCode, struct addrinfo *addr) const;
    };

    typedef std::shared_ptr<HostNameLookup> HostNameLookupPtr;
    typedef std::vector<HostNameLookupPtr> HostNameLookupPtrs;
}

