#include "pch.h"
#include "Logger.h"

namespace Util
{
	Logger::~Logger()
	{
		EndRun();
	}

	void Logger::AttachListener(const Listener* listener)
	{
		assert(listener != nullptr);

		_Listeners.emplace_back(std::unique_ptr<Listener>(const_cast<Listener *>(listener), 
			std::default_delete<Listener>()));
	}

	void Logger::EnqueueLog(const std::string Fmt, LogLevel Level)
	{
		SAFE_UNIQUELOCK(_SwapQueueMutex);

		_InputQueue.emplace(std::tuple<const std::string, LogLevel>(Fmt, Level));
	}

	void Logger::StartRun()
	{
		assert(_Listeners.size() >= 0);

		_LogWriter = std::thread(std::mem_fn(&Logger::DoWork), this);
	}

	void Logger::EndRun()
	{
		_LogWriter.join();
	}

	void Logger::SwapQueue()
	{
		SAFE_UNIQUELOCK(_SwapQueueMutex);

		if (_InputQueue.empty())
			std::this_thread::yield();

		_InputQueue.swap(_WriterQueue);
	}

	void Logger::DoWork()
	{
		while (true) // 차후 이벤트 처리
		{
			if (_WriterQueue.empty())
			{				
				SwapQueue();
				continue;
			}

			const auto& tuple = _WriterQueue.front();

			for (const auto& listener : _Listeners)
			{
				listener->Log(std::get<0>(tuple), std::get<1>(tuple));
			}

			_WriterQueue.pop();
		}
	}
}
