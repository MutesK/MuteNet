#pragma once

#include <memory>
#include "ISocket.h"
#include "WinSocketImpl.h"
#include "LinuxSocketImpl.h"
#include "UnixSocketImpl.h"

namespace MuteNet
{
    class Socket {
    private:
        std::unique_ptr<ISocket> m_pImpl; 

    public:
        Socket() {
#if defined(_WIN32) || defined(_WIN64)
            m_pImpl = std::make_unique<WinSocketImpl>();
#elif defined(__linux__)
            m_pImpl = std::make_unique<LinuxSocketImpl>();
#elif defined(__APPLE__) || defined(__FreeBSD__)
            m_pImpl = std::make_unique<MacSocketImpl>();
#else
#error "지원하지 않는 운영체제입니다."
#endif
        }

        explicit Socket(std::unique_ptr<ISocket> impl) : m_pImpl(std::move(impl)) {}

        ~Socket() = default;

        bool Create(int af = AF_INET, int type = SOCK_STREAM, int protocol = 0) {
            return m_pImpl->Create(af, type, protocol);
        }

        bool Bind(const std::string& ip, uint16_t port) {
            return m_pImpl->Bind(ip, port);
        }

        bool Listen(int backlog = SOMAXCONN) {
            return m_pImpl->Listen(backlog);
        }

        bool Connect(const std::string& ip, uint16_t port) {
            return m_pImpl->Connect(ip, port);
        }

        void Close() {
            m_pImpl->Close();
        }

        bool SetNonBlocking(bool enable) {
            return m_pImpl->SetNonBlocking(enable);
        }

        bool SetReuseAddr(bool enable) {
            return m_pImpl->SetReuseAddress(enable);
        }

        int Send(const void* buffer, size_t length) {
            return m_pImpl->Send(buffer, length);
        }

        int Receive(void* buffer, size_t length) {
            return m_pImpl->Receive(buffer, length);
        }

        int ASyncSend(const void* buffer, size_t length, void* overlappedContext) {
            return m_pImpl->SendAsync(buffer, length, overlappedContext);
        }
     
		int ASyncReceive(void* buffer, size_t length, void* overlappedContext) {
			return m_pImpl->ReceiveAsync(buffer, length, overlappedContext);
		}

        SocketHandle Accept(std::string* clientIp = nullptr, uint16_t* clientPort = nullptr) {
            return m_pImpl->Accept(clientIp, clientPort);
        }

        SocketHandle GetHandle() const {
            return m_pImpl->GetHandle();
        }

        bool IsValid() const {
            return m_pImpl->IsValid();
        }

        bool AcceptAsync(void* acceptor, char* outputBuffer, uint32_t receiveDataLength, void* overlappedContext) {
            return m_pImpl->AcceptAsync(acceptor, outputBuffer, receiveDataLength, overlappedContext);
        }

        void Shutdown(int how) {
            m_pImpl->Shutdown(how);
        }
    };
}