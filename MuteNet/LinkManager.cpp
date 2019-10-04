#include "pch.h"
#include "LinkManager.h"
#include "Link.h"


using namespace Util;
namespace Network
{
	TL::ObjectPool<Link>                   linkPool;
	std::set<const Link*>				   linkSet;
	std::mutex							   linkMutex;

	Link* LinkManager::Alloc()
	{
		auto link = linkPool.Alloc();
		{
			std::lock_guard<std::mutex> lock(linkMutex);
			linkSet.insert(link);
		}
		return link;
	}

	size_t LinkManager::usersize()
	{
		std::lock_guard<std::mutex> lock(linkMutex);

		return linkSet.size();
	}

	void LinkManager::removeSession(const Link* link)
	{
		std::lock_guard<std::mutex> lock(linkMutex);

		auto iter = linkSet.find(link);

		if (iter == linkSet.end())
			return;

		linkSet.erase(iter);
		linkPool.Free(link);
	}

	std::set<const Link*>::iterator LinkManager::unsafe_begin()
	{
		return linkSet.begin();
	}

	std::set<const Link*>::iterator LinkManager::unsafe_end()
	{
		return linkSet.end();
	}
}
