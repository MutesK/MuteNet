//
// Created by Mute on 2020-11-15.
//

#ifndef MUTENET_ATOMICCOUNTER_HPP
#define MUTENET_ATOMICCOUNTER_HPP

#include <atomic>

namespace Util
{
	class AtomicCounter
	{
		std::atomic_int64_t _counter;
	public:
		void IncreaseCounter ( )
		{
			++_counter;
		}
		void DecreaseCounter ( )
		{
			--_counter;
		}
		int64_t GetCounter()
		{
			return _counter;
		}
	};
}

#endif //MUTENET_ATOMICCOUNTER_HPP
