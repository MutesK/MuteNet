#pragma once

#include <atomic>

namespace Util
{
	class ReferenceCounter
	{
	public:
		std::atomic_int_fast64_t _refcount;
	public:
		ReferenceCounter()
			:_refcount(1)
		{}

		void incRefCount()
		{
			_refcount++;
		}

		virtual void decRefCount()
		{
			_refcount--;
		}
	};
}