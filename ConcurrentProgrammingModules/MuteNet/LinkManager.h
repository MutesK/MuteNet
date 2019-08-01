#pragma once

#include <memory>

namespace Network
{
	class Link;
	class LinkManager
	{
	public:
		static std::shared_ptr<Link> make_shared();
	};
}

