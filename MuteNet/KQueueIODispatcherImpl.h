#pragma once
#include "IIODispatcher.h"

#if defined(__APPLE__) || defined(__FreeBSD__)
#include <sys/event.h>
#include <vector>
#include <thread>
#include <atomic>

namespace MuteNet
{
    struct KqueueAsyncContext {
        SocketHandle fd;
        IOOperation op;
        void* buffer;
        size_t length;
        void* customContext;
    };

    class KQueueIODispatcherImpl : public IIODispatcher
    {
    private:
        int m_kq = -1;
        std::vector<std::thread> m_workerThreads;
        std::atomic<bool> m_isRunning{ false };
        IOCallback m_callback;

    public:
        KQueueIODispatcherImpl() = default;
        ~KQueueIODispatcherImpl() override;

        bool Init(size_t threadCount = 0) override;
        void Stop() override;

        bool RegisterSocket(SocketHandle handle, void* userContext) override;
        bool PostAsyncRead(SocketHandle handle, void* buffer, size_t length, void* overlappedOrContext) override;
        bool PostAsyncWrite(SocketHandle handle, const void* buffer, size_t length, void* overlappedOrContext) override;

        void SetCallback(IOCallback callback) override { m_callback = callback; }

    private:
        void WorkerThread();
    };
}
#endif