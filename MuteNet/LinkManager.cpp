#include "pch.h"
#include "LinkManager.h"
#include "Link.h"

using namespace Util;
namespace Network
{
	TL::ObjectPool<Link>                   linkPool;
	std::map<Link*, std::shared_ptr<Link>> linkMap;

	std::shared_ptr<Link> LinkManager::make_shared()
	{
		auto Link = linkPool.make_shared();
		linkMap[Link.get()] = Link;

		return Link;
	}

	size_t LinkManager::UserSize()
	{
		return linkMap.size();
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
