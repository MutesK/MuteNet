#include "LinkManager.h"
#include "../ObjectPool/ObjectPoolTLS.h"
#include "Link.h"

using namespace Util;
namespace Network
{
	TL::ObjectPoolTLS<Link>                   linkPool;
	std::map<LinkPtr, std::shared_ptr<Link>> linkMap;

	std::shared_ptr<Link> LinkManager::make_shared()
	{
		auto Link = linkPool.get_shared();
		linkMap[Link.get()] = Link;

		return Link;
	}

	std::map<Link*, std::shared_ptr<Link>>::iterator LinkManager::begin()
	{
		return linkMap.begin();
	}

	std::map<Link*, std::shared_ptr<Link>>::iterator LinkManager::end()
	{
		return linkMap.end();
	}
}
