#pragma once
#include "Link.h"
#include "../ThreadSafeMemoryPool/MemoryPool.h"

namespace Network
{
	class LinkManager
	{
	public:
		static std::shared_ptr<Link> make_shared();
	};
}

