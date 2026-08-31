#include "LinuxSocketImpl.h"

#if defined(__linux__)
bool MuteNet::LinuxSocketImpl::Create(int af, int type, int protocol)
{
    m_socket = socket(af, type, protocol);
    return m_socket != INVALID_SOCKET_HANDLE;
}

bool MuteNet::LinuxSocketImpl::Bind(const std::string& ip, uint16_t port)
{
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
    return bind(m_socket, (sockaddr*)&addr, sizeof(addr)) == 0;
}

bool MuteNet::LinuxSocketImpl::Listen(int backlog)
{
    return listen(m_socket, backlog) == 0;
}

bool MuteNet::LinuxSocketImpl::Connect(const std::string& ip, uint16_t port)
{
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
    return connect(m_socket, (sockaddr*)&addr, sizeof(addr)) == 0;
}

void MuteNet::LinuxSocketImpl::Close()
{
    if (IsValid()) {
        close(m_socket);
        m_socket = INVALID_SOCKET_HANDLE;
    }
}

bool MuteNet::LinuxSocketImpl::SetNonBlocking(bool enable)
{
    int flags = fcntl(m_socket, F_GETFL, 0);
    if (flags == -1) return false;
    flags = enable ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
    return fcntl(m_socket, F_SETFL, flags) == 0;
}

bool MuteNet::LinuxSocketImpl::SetReuseAddress(bool enable)
{
    int opt = enable ? 1 : 0;
    return setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == 0;
}

bool MuteNet::LinuxSocketImpl::SetNoDelay(bool enable)
{
    return false;
}

int MuteNet::LinuxSocketImpl::Send(const void* buffer, size_t length)
{
    return send(m_socket, buffer, length, MSG_NOSIGNAL); // SIGPIPE 방지
}

int MuteNet::LinuxSocketImpl::Receive(void* buffer, size_t length)
{
    return recv(m_socket, buffer, length, 0);
}

SocketHandle MuteNet::LinuxSocketImpl::Accept(std::string* clientIp, uint16_t* clientPort)
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

SocketHandle MuteNet::LinuxSocketImpl::GetHandle() const
{
    return m_socket;
}

bool MuteNet::LinuxSocketImpl::IsValid() const
{
    return m_socket != INVALID_SOCKET_HANDLE;
}

bool LinuxSocketImpl::SendAsync(const void* buffer, size_t length, void* overlappedContext)
{
    auto* ctx = static_cast<EpollAsyncContext*>(overlappedContext);
    ctx->fd = m_socket;
    ctx->op = IOOperation::Write;
    ctx->buffer = const_cast<void*>(buffer);
    ctx->length = length;

    return Send(buffer, length);
}

bool LinuxSocketImpl::ReceiveAsync(void* buffer, size_t length, void* overlappedContext)
{
    auto* ctx = static_cast<EpollAsyncContext*>(overlappedContext);
    ctx->fd = m_socket;
    ctx->op = IOOperation::Read;
    ctx->buffer = buffer;
    ctx->length = length;
    return true;
}

bool LinuxSocketImpl::AcceptAsync(void* acceptor, char* outputBuffer, uint32_t receiveDataLength, void* overlappedContext)
{
    // Linux epoll 환경에서는 Listen 소켓에 읽기 이벤트(EPOLLIN)가 들어왔을 때 
    // 익명/논블로킹 Accept를 수행하므로 컨텍스트 세팅만 수행합니다.
    auto* ctx = static_cast<EpollAsyncContext*>(overlappedContext);
    ctx->fd = m_socket;
    ctx->op = IOOperation::Read; // Accept는 Read 이벤트 계열로 처리
    ctx->buffer = outputBuffer;
    ctx->length = receiveDataLength;
    return true;
}

void LinuxSocketImpl::Shutdown(int how)
{
    if (IsValid())
    {
        ::shutdown(m_socket, how);
    }
}

bool LinuxSocketImpl::ReceiveFromAsync(void* buffer, size_t length, void* overlappedContext,
    void* addrOut, unsigned int* addrLenOut)
{
    // Linux epoll 기반 비동기 수신 구조 (recvfrom 활용)
    ssize_t bytesRead = recvfrom(
        _socketFd,
        buffer,
        length,
        0,
        (sockaddr*)addrOut,
        addrLenOut
    );

    if (bytesRead < 0) {
        // EAGAIN 또는 EWOULDBLOCK 처리를 통한 논블로킹 대기 연동
        return false;
    }

    return true;
}
#endif