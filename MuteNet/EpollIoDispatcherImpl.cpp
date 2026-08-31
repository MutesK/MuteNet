#include "EpollIODispatcherImpl.h"

#if defined(__linux__)
#include <unistd.h>

namespace MuteNet
{
    EpollIODispatcherImpl::~EpollIODispatcherImpl() {
        Stop();
    }

    bool EpollIODispatcherImpl::Init(size_t threadCount) {
        m_epollFd = epoll_create1(0);
        if (m_epollFd == -1) return false;

        if (threadCount == 0) threadCount = std::thread::hardware_concurrency() * 2;

        m_isRunning = true;
        for (size_t i = 0; i < threadCount; ++i) {
            m_workerThreads.emplace_back(&EpollIODispatcherImpl::WorkerThread, this);
        }
        return true;
    }

    void EpollIODispatcherImpl::Stop() {
        if (!m_isRunning) return;
        m_isRunning = false;

        for (auto& t : m_workerThreads) {
            if (t.joinable()) t.join();
        }
        m_workerThreads.clear();

        if (m_epollFd != -1) {
            close(m_epollFd);
            m_epollFd = -1;
        }
    }

    bool EpollIODispatcherImpl::RegisterSocket(SocketHandle handle, void* userContext) {
        epoll_event ev{};
        ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
        ev.data.ptr = userContext;
        return epoll_ctl(m_epollFd, EPOLL_CTL_ADD, handle, &ev) == 0;
    }

    bool EpollIODispatcherImpl::PostAsyncRead(SocketHandle handle, void* buffer, size_t length, void* overlappedOrContext) {
        auto* ctx = static_cast<EpollAsyncContext*>(overlappedOrContext);
        ctx->fd = handle;
        ctx->op = IOOperation::Read;
        ctx->buffer = buffer;
        ctx->length = length;
        return true;
    }

    bool EpollIODispatcherImpl::PostAsyncWrite(SocketHandle handle, const void* buffer, size_t length, void* overlappedOrContext) {
        auto* ctx = static_cast<EpollAsyncContext*>(overlappedOrContext);
        ctx->fd = handle;
        ctx->op = IOOperation::Write;
        ctx->buffer = const_cast<void*>(buffer);
        ctx->length = length;
        return true;
    }

    void EpollIODispatcherImpl::WorkerThread() {
        const int MAX_EVENTS = 64;
        epoll_event events[MAX_EVENTS];

        while (m_isRunning) {
            int nfds = epoll_wait(m_epollFd, events, MAX_EVENTS, 100);
            for (int i = 0; i < nfds; ++i) {
                auto* ctx = static_cast<EpollAsyncContext*>(events[i].data.ptr);
                if (!ctx) continue;

                if (events[i].events & EPOLLIN) {
                    ssize_t bytes = recv(ctx->fd, ctx->buffer, ctx->length, MSG_DONTWAIT);
                    bool success = (bytes > 0);
                    if (m_callback) m_callback(success, success ? static_cast<uint32_t>(bytes) : 0, IOOperation::Read, ctx->customContext);
                }

                if (events[i].events & EPOLLOUT) {
                    ssize_t bytes = send(ctx->fd, ctx->buffer, ctx->length, MSG_DONTWAIT | MSG_NOSIGNAL);
                    bool success = (bytes >= 0);
                    if (m_callback) m_callback(success, success ? static_cast<uint32_t>(bytes) : 0, IOOperation::Write, ctx->customContext);
                }
            }
        }
    }
}

#endif