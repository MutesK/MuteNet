#include "pch.h"
#include "LinkManager.h"
#include "Link.h"

using namespace Util;
namespace Network
{
	TL::ObjectPool<Link>                   linkPool;
	std::map<Link*, std::shared_ptr<Link>> linkMap;

	std::shared_ptr<Link> LinkManager::make_shared(std::shared_ptr<TcpSocket>& socket, ConnectPoint& endPoint)
	{
		auto Link = linkPool.make_shared(socket, endPoint);
		linkMap[Link.get()] = Link;

		return Link;
	}

	void LinkManager::disconnect_link(const std::shared_ptr<Link> link)
	{
		const auto iter = linkMap.find(link.get());
		linkMap.erase(iter);
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
