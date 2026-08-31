#include "KqueueIODispatcherImpl.h"

#if defined(__APPLE__) || defined(__FreeBSD__)
#include <unistd.h>
#include <sys/time.h>

KQueueIODispatcherImpl::~KQueueIODispatcherImpl() {
    Stop();
}

bool KQueueIODispatcherImpl::Init(size_t threadCount) {
    m_kq = kqueue();
    if (m_kq == -1) return false;

    if (threadCount == 0) threadCount = std::thread::hardware_concurrency() * 2;

    m_isRunning = true;
    for (size_t i = 0; i < threadCount; ++i) {
        m_workerThreads.emplace_back(&KQueueIODispatcherImpl::WorkerThread, this);
    }
    return true;
}

void KQueueIODispatcherImpl::Stop() {
    if (!m_isRunning) return;
    m_isRunning = false;

    for (auto& t : m_workerThreads) {
        if (t.joinable()) t.join();
    }
    m_workerThreads.clear();

    if (m_kq != -1) {
        close(m_kq);
        m_kq = -1;
    }
}

bool KQueueIODispatcherImpl::RegisterSocket(SocketHandle handle, void* userContext) {
    struct kevent ev[2];
    EV_SET(&ev[0], handle, EVFILT_READ, EV_ADD | EV_ENABLE | EV_CLEAR, 0, 0, userContext);
    EV_SET(&ev[1], handle, EVFILT_WRITE, EV_ADD | EV_ENABLE | EV_CLEAR, 0, 0, userContext);
    return kevent(m_kq, ev, 2, NULL, 0, NULL) == 0;
}

bool KQueueIODispatcherImpl::PostAsyncRead(SocketHandle handle, void* buffer, size_t length, void* overlappedOrContext) {
    auto* ctx = static_cast<KqueueAsyncContext*>(overlappedOrContext);
    ctx->fd = handle;
    ctx->op = IOOperation::Read;
    ctx->buffer = buffer;
    ctx->length = length;
    return true;
}

bool KQueueIODispatcherImpl::PostAsyncWrite(SocketHandle handle, const void* buffer, size_t length, void* overlappedOrContext) {
    auto* ctx = static_cast<KqueueAsyncContext*>(overlappedOrContext);
    ctx->fd = handle;
    ctx->op = IOOperation::Write;
    ctx->buffer = const_cast<void*>(buffer);
    ctx->length = length;
    return true;
}

void KQueueIODispatcherImpl::WorkerThread() {
    const int MAX_EVENTS = 64;
    struct kevent events[MAX_EVENTS];
    struct timespec timeout { 0, 100000000 }; // 100ms

    while (m_isRunning) {
        int nfds = kevent(m_kq, NULL, 0, events, MAX_EVENTS, &timeout);
        for (int i = 0; i < nfds; ++i) {
            auto* ctx = static_cast<KqueueAsyncContext*>(events[i].udata);
            if (!ctx) continue;

            if (events[i].filter == EVFILT_READ) {
                ssize_t bytes = recv(ctx->fd, ctx->buffer, ctx->length, MSG_DONTWAIT);
                bool success = (bytes > 0);
                if (m_callback) m_callback(success, success ? static_cast<uint32_t>(bytes) : 0, IOOperation::Read, ctx->customContext);
            }

            if (events[i].filter == EVFILT_WRITE) {
                ssize_t bytes = send(ctx->fd, ctx->buffer, ctx->length, 0);
                bool success = (bytes >= 0);
                if (m_callback) m_callback(success, success ? static_cast<uint32_t>(bytes) : 0, IOOperation::Write, ctx->customContext);
            }
        }
    }
}

#endif