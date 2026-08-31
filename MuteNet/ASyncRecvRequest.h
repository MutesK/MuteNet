#pragma once

#include "ASyncIORequest.h"
#include "Link.h"
#include "Socket.h"

namespace MuteNet
{
    class ASyncRecvRequest : public ASyncIORequest
    {
        LinkPtr link;
        std::shared_ptr<Socket> socket;
        char* recvBuffer;     // 외부에서 제공받거나 내부 할당된 버퍼
        size_t bufferSize;

    public:
        ASyncRecvRequest(LinkPtr link, std::shared_ptr<Socket> socket, size_t bufferSize = 8192);

        virtual ~ASyncRecvRequest();

        static ASyncRecvRequest* GetRecvRequest(LinkPtr link, std::shared_ptr<Socket> socket, size_t bufferSize = 8192);
        static void FreeRecvRequest(ASyncRecvRequest* request);

        void OnComplete(uint32_t transferredBytes, IOOperation op) override;
        void OnError(IOOperation op) override;

        bool Process() override;
    };
}