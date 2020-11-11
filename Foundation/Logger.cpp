#include "FoundationCommon.h"
#include "Logger.h"

namespace Util
{
    Logger::Logger()
    {
    }

    Logger::~Logger()
    {
        EndRun();
    }

    void Logger::AttachListener(const Listener *listener)
    {
        assert(listener != nullptr);

        _Listeners.emplace_back(std::unique_ptr<Listener>(const_cast<Listener *>(listener),
                                                          std::default_delete<Listener>()));
    }

    void Logger::EnqueueLog(const std::string Fmt, ELogLevel Level)
    {
        // SetEvent();

        SAFE_UNIQUELOCK(_SwapQueueMutex);

        _InputQueue.emplace(std::tuple<const std::string, ELogLevel>(Fmt, Level));

        // SetEvent();
    }

    void Logger::StartRun()
    {
        assert(_Listeners.size() >= 0);

        // SetEvent();
    }

    void Logger::EndRun()
    {
        // Finalize();
    }

    void Logger::SwapQueue()
    {
        SAFE_UNIQUELOCK(_SwapQueueMutex);

        if (_InputQueue.empty())
        {
            std::this_thread::yield();
        }

        _InputQueue.swap(_WriterQueue);
    }

    void Logger::DoWork()
    {
        if (_WriterQueue.empty())
        {
            SwapQueue();
        }

        while (!_WriterQueue.empty())
        {
            const auto &tuple = _WriterQueue.front();
            for (const auto &listener : _Listeners)
            {
                listener->Log(std::get<0>(tuple), std::get<1>(tuple));
            }

            _WriterQueue.pop();

            if (_WriterQueue.empty())
            {
                SwapQueue();
            }
        }
    }
}
