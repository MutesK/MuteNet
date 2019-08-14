
#include "Link.h"
#include "IOContext.h"


namespace Network
{
	Util::TL::ObjectPool<SendContext> SendContext::OverlappedPool;
	Util::TL::ObjectPool<RecvContext> RecvContext::OverlappedPool;
	Util::TL::ObjectPool<AcceptContext> AcceptContext::OverlappedPool;
	Util::TL::ObjectPool<ConnectContext> ConnectContext::OverlappedPool;
}