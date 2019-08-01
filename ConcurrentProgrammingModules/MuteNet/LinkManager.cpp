#include "LinkManager.h"

namespace Network
{
	Util::MemoryPool<Link, 5000>			  linkPool;
	std::map<LinkPtr, std::shared_ptr<Link>&> linkMap;

	std::shared_ptr<Link> LinkManager::make_shared()
	{
		auto Link = linkPool.make_shared();
		linkMap[Link.get()] = Link;

		return Link;
	}
}
