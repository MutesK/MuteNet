#pragma once

#include <memory>
#include <map>
#include <set>

namespace Network
{
	class Link;
	class LinkManager
	{
	public:
		static Link* Alloc();
		static size_t usersize();

		static void removeSession(const Link* link);

		static std::set<const Link*>::iterator unsafe_begin();
		static std::set<const Link*>::iterator unsafe_end();
	};
}

