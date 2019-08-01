#include "LinkManager.h"
#include "../ObjectPool/ObjectPoolTLS.h"
#include "Link.h"

using namespace Util;
namespace Network
{
	ObjectPoolTLS<Link>                       linkPool;
	std::map<LinkPtr, std::shared_ptr<Link>&> linkMap;

	std::shared_ptr<Link> LinkManager::make_shared()
	{
		auto Link = linkPool.get_shared();
		linkMap[Link.get()] = Link;

		return Link;
	}
}
