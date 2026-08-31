#pragma once

#include <memory>
#include <string>
#include <functional>
#include "Link.h"

namespace MuteNet
{
    class Socket;
    class IODispatcher;
    class ASyncAcceptRequest;

    class Acceptor final : public std::enable_shared_from_this<Acceptor>
    {
    public:
        // 클라이언트 수락 성공 시 호출될 콜백 타입
        using AcceptCallback = std::function<void(std::shared_ptr<Socket> clientSocket)>;
        // 에러 발생 시 호출될 콜백 타입
        using ErrorCallback = std::function<void(int errorCode, const std::string& errorMsg)>;

    private:
        std::shared_ptr<Socket> listenSocket;
        std::shared_ptr<IODispatcher> ioDispatcher;

        AcceptCallback acceptCallback;
        ErrorCallback errorCallback;
        bool isListening = false;

    public:
        explicit Acceptor(std::shared_ptr<IODispatcher> dispatcher);
        ~Acceptor();

        bool Listen(const std::string& ip, uint16_t port, int backlog = 0x7fffffff);
        void StartAccept();
        void Close();

        void SetCallbacks(AcceptCallback onAccept, ErrorCallback onError);

        // ASyncAcceptRequest에서 호출할 내부 완료/에러 핸들러
        void OnAcceptCompleted(std::shared_ptr<Socket> clientSocket, char* addressBuffer, uint32_t transferredBytes);
        void OnAcceptError(std::shared_ptr<Socket> clientSocket);

    private:
        friend class ASyncAcceptRequest;
    };

    using AcceptorPtr = std::shared_ptr<Acceptor>;
}