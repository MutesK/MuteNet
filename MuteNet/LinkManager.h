#pragma once

#include <memory>
#include <map>

namespace Network
{
	class Link;
	class LinkManager
	{
	public:
		static std::shared_ptr<Link> make_shared();

		static std::map<Link*, std::shared_ptr<Link>>::iterator begin();
		static std::map<Link*, std::shared_ptr<Link>>::iterator end();
	};
}

