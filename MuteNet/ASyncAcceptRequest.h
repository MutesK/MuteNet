#pragma once

#include "ASyncIORequest.h"
#include <memory>

#ifdef _WIN32
#include <winsock2.h>
#include <mswsock.h>
#endif

namespace MuteNet
{
    class Socket;
    class Acceptor; // Acceptor 클래스 전방 선언 (또는 관련 콜백 인터페이스)

    class ASyncAcceptRequest : public ASyncIORequest
    {
        std::shared_ptr<Acceptor> acceptor;
        std::shared_ptr<Socket> clientSocket;
        char addressBuffer[1024];

    public:
        ASyncAcceptRequest(std::shared_ptr<Acceptor> acceptor, std::shared_ptr<Socket> clientSocket);
        virtual ~ASyncAcceptRequest();

        static ASyncAcceptRequest* GetAcceptRequest(std::shared_ptr<Acceptor> acceptor, std::shared_ptr<Socket> clientSocket);
        static void FreeAcceptRequest(ASyncAcceptRequest* request);

        void OnComplete(uint32_t transferredBytes, IOOperation op) override;
        void OnError(IOOperation op) override;
        bool Process() override;
    };
}