
#include "Link.h"


namespace Network
{
	Link::Link(const std::shared_ptr<TcpSocket>& socket)
		:_refCount(0)
	{
	}

	Link::Link(const TcpSocket* socket)
		: _refCount(0)
	{
	}

	Link::~Link()
	{
	}

	void Link::AddRef()
	{
		++_refCount;
	}

	void Link::Release()
	{
		--_refCount;

		if (_refCount <= 0)
			delete this;
	}
}
