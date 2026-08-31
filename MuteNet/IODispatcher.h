#pragma once

#include <memory>
#include "IIODispatcher.h"
#include "WinIODispatcherImpl.h"
#include "EpollIODispatcherImpl.h"
#include "KQueueIODispatcherImpl.h"

namespace MuteNet
{
    class IODispatcher 
    {
    private:
        std::unique_ptr<IIODispatcher> m_pImpl;

    public:
        IODispatcher() {
#if defined(_WIN32) || defined(_WIN64)
            m_pImpl = std::make_unique<WinIODispatcherImpl>();
#elif defined(__linux__)
            m_pImpl = std::make_unique<EpollIODispatcherImpl>();
#elif defined(__APPLE__) || defined(__FreeBSD__)
            m_pImpl = std::make_unique<KQueueIODispatcherImpl>();
#else
#error "지원하지 않는 OS입니다."
#endif
        }

        ~IODispatcher() = default;

        bool Init(size_t threadCount = 0) {
            return m_pImpl->Init(threadCount);
        }

        void Stop() {
            m_pImpl->Stop();
        }

        bool RegisterSocket(SocketHandle handle, void* userContext) {
            return m_pImpl->RegisterSocket(handle, userContext);
        }

        bool PostAsyncRead(SocketHandle handle, void* buffer, size_t length, void* overlappedOrContext) {
            return m_pImpl->PostAsyncRead(handle, buffer, length, overlappedOrContext);
        }

        bool PostAsyncWrite(SocketHandle handle, const void* buffer, size_t length, void* overlappedOrContext) {
            return m_pImpl->PostAsyncWrite(handle, buffer, length, overlappedOrContext);
        }

        void SetCallback(IOCallback callback) {
            m_pImpl->SetCallback(callback);
        }
    };
}