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

	}

}