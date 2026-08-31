#pragma once
#include "IIODispatcher.h"

#if defined(__linux__)
#include <sys/epoll.h>
#include <vector>
#include <thread>
#include <atomic>

namespace MuteNet
{
    struct EpollAsyncContext {
        SocketHandle fd;
        IOOperation op;
        void* buffer;
        size_t length;
        void* customContext;
    };

    class EpollIODispatcherImpl : public IIODispatcher {
    private:
        int m_epollFd = -1;
        std::vector<std::thread> m_workerThreads;
        std::atomic<bool> m_isRunning{ false };
        IOCallback m_callback;

    public:
        EpollIODispatcherImpl() = default;
        ~EpollIODispatcherImpl() override;

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