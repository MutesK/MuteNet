//
// Created by JunMin Kim on 2020/11/11.
//

#ifndef MUTENET_EVENT_HPP
#define MUTENET_EVENT_HPP

namespace Util
{
	class Event
	{
		bool _shouldContinue;
		std::mutex _mutex;
		std::condition_variable _condVar;
	public:
		Event();

		void Wait();

		void Set();

		void SetAll();

		bool Wait(uint32_t TimeoutMsec);
	};
}

#endif //MUTENET_EVENT_HPP
