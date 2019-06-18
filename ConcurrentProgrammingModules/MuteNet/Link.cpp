
#include "Link.h"

namespace Network
{
	Link::Link()
		:_refCount(0)
	{
	}

	EndPoint* Link::GetEndPoint(Handle handle)
	{
		LinkPtr Link(handle);

		return &Link->_socket->_endPoint;
	}

	void Link::ForceDisconnect(Handle handle)
	{
		LinkPtr Link(handle);

		Link->_socket->Shutdown(ShutdownBlockMode::SendBlock);
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

	void* Link::native_handle()
	{
		return reinterpret_cast<void*>(_socket->_handle);
	}
}
