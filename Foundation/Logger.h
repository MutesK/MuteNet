#pragma once

#include "Singleton.h"
#include "SafeSharedLock.h"

namespace Util
{
	enum class LogLevel
	{
		Debug,
		Warning,
		Error,
		Fatal,
		Info,
	};

	class Logger : public TL::Singleton<Logger>
	{
	public:

		class Listener abstract
		{
		public:
			virtual void Log(const std::string logFmt, LogLevel level) = 0;
		};

		friend class LogHelper;
		friend class TL::Singleton<Logger>;
	private:
		SafeSharedLock	_SwapQueueMutex;
		std::thread _LogWriter;
			
		std::queue<std::tuple<const std::string, LogLevel>> _InputQueue;
		std::queue<std::tuple<const std::string, LogLevel>> _WriterQueue;

		std::list<std::unique_ptr<Listener>>    _Listeners;
	protected:
		Logger() = default;

	public:
		virtual ~Logger();

		void AttachListener(const Listener* listener);

		void EnqueueLog(const std::string Fmt, LogLevel level);
		void StartRun();
		void EndRun();
	private:
		void SwapQueue();

		void DoWork();
	};
}

