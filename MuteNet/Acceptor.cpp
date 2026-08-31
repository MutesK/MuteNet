#include "Common.h"
#include "Acceptor.h"
#include "Socket.h"
#include "IODispatcher.h"
#include "ASyncAcceptRequest.h"
#include "Allocator.h"
#include <stdexcept>

namespace MuteNet
{
    Acceptor::Acceptor(std::shared_ptr<IODispatcher> dispatcher)
        : ioDispatcher(std::move(dispatcher))
    {
        auto rawSock = Allocator::New<Socket>();
        listenSocket = std::shared_ptr<Socket>(rawSock, [](Socket* ptr) {
            Allocator::Delete(ptr);
            });

        listenSocket->Create(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }

    Acceptor::~Acceptor()
    {
        Close();
    }

    bool Acceptor::Listen(const std::string& ip, uint16_t port, int backlog)
    {
        if (!listenSocket || !listenSocket->IsValid())
            return false;

        if (!listenSocket->Bind(ip, port))
            return false;

        if (!listenSocket->Listen(backlog))
            return false;

        if (ioDispatcher)
        {
            ioDispatcher->RegisterSocket(listenSocket->GetHandle(), this);
        }
        else
        {
            throw std::runtime_error("IODispatcher is not set in Acceptor.");
        }

        isListening = true;
        StartAccept(); // 첫 비동기 Accept 걸기
        return true;
    }

    void Acceptor::StartAccept()
    {
        if (!isListening)
            return;

        // 클라이언트 소켓 생성
        auto rawClientSock = Allocator::New<Socket>();
        auto clientSocket = std::shared_ptr<Socket>(rawClientSock, [](Socket* ptr) {
            Allocator::Delete(ptr);
            });
        clientSocket->Create(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        // 비동기 Accept 요청 객체 생성 및 프로세스 시작
        auto acceptReq = ASyncAcceptRequest::GetAcceptRequest(shared_from_this(), clientSocket);
        if (!acceptReq->Process())
        {
            ASyncAcceptRequest::FreeAcceptRequest(acceptReq);
            // 에러 처리 후 잠시 뒤 재시도 하거나 종료 처리
        }
    }

    void Acceptor::Close()
    {
        if (!isListening)
            return;

        isListening = false;
        if (listenSocket)
        {
            listenSocket->Close();
        }
    }

    void Acceptor::SetCallbacks(AcceptCallback onAccept, ErrorCallback onError)
    {
        acceptCallback = std::move(onAccept);
        errorCallback = std::move(onError);
    }

    void Acceptor::OnAcceptCompleted(std::shared_ptr<Socket> clientSocket, char* addressBuffer, uint32_t transferredBytes)
    {
        // 1. 상위로 수락된 클라이언트 소켓 전달
        if (acceptCallback)
        {
            acceptCallback(clientSocket);
        }

        // 2. 다음 클라이언트 수락을 위해 대기 루프 지속
        StartAccept();
    }

    void Acceptor::OnAcceptError(std::shared_ptr<Socket> clientSocket)
    {
        if (errorCallback)
        {
            errorCallback(-1, "Accept operation failed.");
        }

        // 에러가 나더라도 서버가 죽지 않고 다음 Accept를 받도록 할 수 있음
        StartAccept();
    }
}