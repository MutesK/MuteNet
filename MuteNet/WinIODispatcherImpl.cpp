#include "WinIODispatcherImpl.h"

#if defined(_WIN32) || defined(_WIN64)

MuteNet::WinIODispatcherImpl::~WinIODispatcherImpl()
{
    Stop();
}

bool MuteNet::WinIODispatcherImpl::Init(size_t threadCount)
{
    IocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
    if (!IocpHandle)
        return false;

    if (threadCount <= 0)
    {
        threadCount = std::thread::hardware_concurrency() * 2;
    }

    IsRunning = true;
    for (auto i = 0;i < threadCount; ++i)
    {
        WorkerThreads.emplace_back(&WinIODispatcherImpl::WorkerThread, this);
    }

    return true;
}

void MuteNet::WinIODispatcherImpl::Stop()
{
    if (!IsRunning)
        return;

    IsRunning = false;

    for (size_t i = 0; i < WorkerThreads.size(); ++i)
    {
        PostQueuedCompletionStatus(IocpHandle, 0, 0, nullptr);
    }

    for (auto& t : WorkerThreads)
    {
        if (t.joinable())
            t.join();
    }

    WorkerThreads.clear();
    if (IocpHandle)
    {
        CloseHandle(IocpHandle);
        IocpHandle = 0;
    }
}

bool MuteNet::WinIODispatcherImpl::RegisterSocket(SocketHandle handle, void* UserContext)
{
    return CreateIoCompletionPort((HANDLE)handle,
        IocpHandle, (ULONG_PTR)UserContext, 0) != nullptr;
}

bool MuteNet::WinIODispatcherImpl::PostAsyncRead(SocketHandle handle, void* buffer, size_t length, void* overlappedContext)
{
    auto* ctx = static_cast<OverlappedContext*>(overlappedContext);
    ZeroMemory(&ctx->Overlapped, sizeof(WSAOVERLAPPED));
    ctx->Op = IOOperation::Read;
    ctx->wsabuf.buf = (CHAR*)buffer;
    ctx->wsabuf.len = (ULONG)length;

    DWORD bytesRecv = 0, flags = 0;
    int ret = WSARecv(handle, &ctx->wsabuf, 1, &bytesRecv, &flags, &ctx->Overlapped, NULL);
    if (ret == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING) {
        return false;
    }
    return true;
}

bool MuteNet::WinIODispatcherImpl::PostAsyncWrite(SocketHandle handle, const void* buffer, size_t length, void* overlappedContext)
{
    auto* ctx = static_cast<OverlappedContext*>(overlappedContext);
    ZeroMemory(&ctx->Overlapped, sizeof(WSAOVERLAPPED));
    ctx->Op = IOOperation::Write;
    ctx->wsabuf.buf = (CHAR*)buffer;
    ctx->wsabuf.len = (ULONG)length;

    DWORD bytesSent = 0;
    int ret = WSASend(handle, &ctx->wsabuf, 1, &bytesSent, 0, &ctx->Overlapped, NULL);
    if (ret == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING) {
        return false;
    }
    return true;
}

void MuteNet::WinIODispatcherImpl::SetCallback(IOCallback callback)
{
    Callback = callback;
}

void MuteNet::WinIODispatcherImpl::WorkerThread()
{
    DWORD bytesTransfferd = 0;
    ULONG_PTR Completionkey = 0;
    LPOVERLAPPED pOverlapped = nullptr;

    while (IsRunning)
    {
        auto result = GetQueuedCompletionStatus(IocpHandle, &bytesTransfferd,
            &Completionkey, &pOverlapped, INFINITE);

        if (!pOverlapped)
            break; // 종료 시그널 

        auto* ctx = reinterpret_cast<OverlappedContext*>(pOverlapped);
        bool success = result && (bytesTransfferd > 0 || ctx->Op == IOOperation::Accept);

        if (Callback)
        {
            Callback(success, bytesTransfferd, ctx->Op, ctx->Context);
        }
    }
}

#endif
