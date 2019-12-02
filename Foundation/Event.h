#pragma once


namespace Util
{
	class Event
	{
		bool					_shouldContinue;
		std::mutex				 _mutex;
		std::condition_variable  _condVar;
	public:
		Event();

		void Wait();
		void Set();
		void SetAll();
		bool Wait(uint32_t TimeoutMsec);
	};
}


