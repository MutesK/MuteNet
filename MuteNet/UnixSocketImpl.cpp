#include "UnixSocketImpl.h"

#if defined(__APPLE__) || defined(__FreeBSD__)

bool MuteNet::UnixSocketImpl::Create(int af, int type, int protocol)
{
    m_socket = socket(af, type, protocol);
    return m_socket != INVALID_SOCKET_HANDLE;
}

bool MuteNet::UnixSocketImpl::Bind(const std::string& ip, uint16_t port)
{
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
    return bind(m_socket, (sockaddr*)&addr, sizeof(addr)) == 0;
}

bool MuteNet::UnixSocketImpl::Listen(int backlog)
{
    return listen(m_socket, backlog) == 0;
}

bool MuteNet::UnixSocketImpl::Connect(const std::string& ip, uint16_t port)
{
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
    return connect(m_socket, (sockaddr*)&addr, sizeof(addr)) == 0;
}

void MuteNet::UnixSocketImpl::Close()
{
    if (IsValid()) {
        close(m_socket);
        m_socket = INVALID_SOCKET_HANDLE;
    }
}

bool MuteNet::UnixSocketImpl::SetNonBlocking(bool enable)
{
    int flags = fcntl(m_socket, F_GETFL, 0);
    if (flags == -1) return false;
    flags = enable ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
    return fcntl(m_socket, F_SETFL, flags) == 0;
}

bool MuteNet::UnixSocketImpl::SetReuseAddress(bool enable)
{
    int opt = enable ? 1 : 0;
    return setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == 0;
}

bool MuteNet::UnixSocketImpl::SetNoDelay(bool enable)
{
    return false;
}

int MuteNet::UnixSocketImpl::Send(const void* buffer, size_t length)
{
    return send(m_socket, buffer, length, 0);
}

int MuteNet::UnixSocketImpl::Receive(void* buffer, size_t length)
{
    return recv(m_socket, buffer, length, 0);
}

SocketHandle MuteNet::UnixSocketImpl::Accept(std::string* clientIp, uint16_t* clientPort)
{
    sockaddr_in addr{};
    socklen_t addrLen = sizeof(addr);
    SocketHandle client = accept(m_socket, (sockaddr*)&addr, &addrLen);
    if (client != INVALID_SOCKET_HANDLE && clientIp) {
        char ipStr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &addr.sin_addr, ipStr, sizeof(ipStr));
        *clientIp = ipStr;
        if (clientPort) *clientPort = ntohs(addr.sin_port);
    }
    return client;
}

SocketHandle MuteNet::UnixSocketImpl::GetHandle() const
{
    return m_socket;
}

bool MuteNet::UnixSocketImpl::IsValid() const
{
    return m_socket != INVALID_SOCKET_HANDLE;
}

bool UnixSocketImpl::SendAsync(const void* buffer, size_t length, void* overlappedContext)
{
    auto* ctx = static_cast<KqueueAsyncContext*>(overlappedContext);
    ctx->fd = m_socket;
    ctx->op = IOOperation::Write;
    ctx->buffer = const_cast<void*>(buffer);
    ctx->length = length;
    return true;
}

bool UnixSocketImpl::ReceiveAsync(void* buffer, size_t length, void* overlappedContext)
{
    auto* ctx = static_cast<KqueueAsyncContext*>(overlappedContext);
    ctx->fd = m_socket;
    ctx->op = IOOperation::Read;
    ctx->buffer = buffer;
    ctx->length = length;
    return true;
}

bool UnixSocketImpl::AcceptAsync(void* acceptor, char* outputBuffer, uint32_t receiveDataLength, void* overlappedContext)
{
    auto* ctx = static_cast<KqueueAsyncContext*>(overlappedContext);
    ctx->fd = m_socket;
    ctx->op = IOOperation::Read;
    ctx->buffer = outputBuffer;
    ctx->length = receiveDataLength;
    return true;
}

void UnixSocketImpl::Shutdown(int how)
{
    if (IsValid())
    {
        ::shutdown(m_socket, how);
    }
}

bool UnixSocketImpl::ReceiveFromAsync(void* buffer, size_t length, void* overlappedContext,
    void* addrOut, unsigned int* addrLenOut)
{
    // macOS kqueue 이벤트 통지 후 recvfrom 호출
    ssize_t bytesRead = recvfrom(
        _socketFd,
        buffer,
        length,
        0,
        (sockaddr*)addrOut,
        addrLenOut
    );

    if (bytesRead < 0) {
        return false;
    }

    return true;
}
#endif