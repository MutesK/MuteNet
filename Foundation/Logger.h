#pragma once

#include "Singleton.h"
#include "SafeSharedLock.h"
#include "Runnable.hpp"

namespace Util
{
    enum class ELogLevel
    {
        Debug,
        Warning,
        Error,
        Fatal,
        Info,
    };

    class Logger : public TL::Singleton<Logger>, Runnable
    {
        typedef Runnable super;
    public:
        class Listener abstract
        {
        public:
            virtual ~Listener() = default;

            virtual void Log(const std::string logFmt, ELogLevel level) = 0;
        };

        friend class LogHelper;

        friend class TL::Singleton<Logger>;

    private:
        SafeSharedLock _SwapQueueMutex;

        std::queue<std::tuple<const std::string, ELogLevel>> _InputQueue;
        std::queue<std::tuple<const std::string, ELogLevel>> _WriterQueue;

        std::list<std::unique_ptr<Listener>> _Listeners;

    protected:
        Logger();

    public:
        virtual ~Logger();

        void AttachListener(const Listener *listener);

        void EnqueueLog(const std::string Fmt, ELogLevel level);

        void StartRun();

        void EndRun();

    private:
        void SwapQueue();

        virtual void DoWork() override;
    };
}

