#include "ASyncRecvRequest.h"
#include "Allocator.h"

namespace MuteNet
{
    ASyncRecvRequest::ASyncRecvRequest(LinkPtr link, std::shared_ptr<Socket> socket, size_t bufferSize)
        : link(std::move(link)), socket(std::move(socket)), bufferSize(bufferSize)
    {
        recvBuffer = static_cast<char*>(Allocator::GetResource().allocate(bufferSize, 64));
    }

    ASyncRecvRequest::~ASyncRecvRequest()
    {
        // 외부 버퍼가 아닐 때만 Allocator를 통해 해제
        if (recvBuffer)
        {
            Allocator::GetResource().deallocate(recvBuffer, bufferSize, 64);
            recvBuffer = nullptr;
        }
    }

    ASyncRecvRequest* ASyncRecvRequest::GetRecvRequest(LinkPtr link, std::shared_ptr<Socket> socket, size_t bufferSize)
    {
        return Allocator::New<ASyncRecvRequest>(link, socket, bufferSize);
    }

    void ASyncRecvRequest::FreeRecvRequest(ASyncRecvRequest* request)
    {
        Allocator::Delete(request);
    }

    bool ASyncRecvRequest::Process()
    {
        if (!socket || !socket->IsValid() || !recvBuffer)
            return false;
        
        return socket->ASyncReceive(recvBuffer, bufferSize, GetPlatformContext());
    }

    void ASyncRecvRequest::OnComplete(uint32_t transferredBytes, IOOperation op)
    {
        auto callbacks = link->GetCallbacks();
        if (transferredBytes == 0)
        {
            link->Close();
            FreeRecvRequest(this);
            return;
        }
        
        if (callbacks)
        {
            // 수신된 데이터를 상위 콜백으로 전달
            callbacks->OnReceived(link.get(), recvBuffer, transferredBytes);
        }


        FreeRecvRequest(this);
       
    }

    void ASyncRecvRequest::OnError(IOOperation op)
    {
        link->Close();
        FreeRecvRequest(this);
    }
}