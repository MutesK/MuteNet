#include "ASyncAcceptRequest.h"
#include "Allocator.h"
#include "Socket.h"
#include "Acceptor.h"

namespace MuteNet
{
    ASyncAcceptRequest::ASyncAcceptRequest(std::shared_ptr<Acceptor> acceptor, std::shared_ptr<Socket> clientSocket)
        : acceptor(acceptor), clientSocket(clientSocket), addressBuffer{ 0 }
    {
    }

    ASyncAcceptRequest::~ASyncAcceptRequest()
    {
    }

    ASyncAcceptRequest* ASyncAcceptRequest::GetAcceptRequest(std::shared_ptr<Acceptor> acceptor, std::shared_ptr<Socket> clientSocket)
    {
        return Allocator::New<ASyncAcceptRequest>(acceptor, clientSocket);
    }

    void ASyncAcceptRequest::FreeAcceptRequest(ASyncAcceptRequest* request)
    {
        Allocator::Delete(request);
    }

    bool ASyncAcceptRequest::Process()
    {
        if (!acceptor || !clientSocket)
            return false;

        return clientSocket->AcceptAsync(acceptor->listenSocket.get(), addressBuffer, sizeof(addressBuffer), GetPlatformContext());
    }

    void ASyncAcceptRequest::OnComplete(uint32_t transferredBytes, IOOperation op)
    {
        // 1. 수락 완료 후 후속 작업 처리 (예: SO_UPDATE_ACCEPT_CONTEXT 및 콜백 호출)
        if (acceptor)
        {
            acceptor->OnAcceptCompleted(clientSocket, addressBuffer, transferredBytes);
        }

        // 2. 요청 객체 메모리 해제
        FreeAcceptRequest(this);
    }

    void ASyncAcceptRequest::OnError(IOOperation op)
    {
        if (acceptor)
        {
            acceptor->OnAcceptError(clientSocket);
        }

        FreeAcceptRequest(this);
    }
}