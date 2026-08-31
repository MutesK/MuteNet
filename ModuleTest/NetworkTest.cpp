
#include "NetworkTest.h"

#include <Acceptor.h>
#include <TCPLinkImpl.h>
#include <UDPLinkImpl.h>
#include <Socket.h>
#include <IODispatcher.h>
#include <thread>
#include <chrono>
#include <atomic>
#include <memory>
#include <vector>
#include <string>

// ALink::Callbacks의 모든 순수 가상 함수를 구현한 테스트용 콜백 클래스
class TestLinkCallbacks : public MuteNet::ALink::Callbacks {
public:
    std::atomic<int> receivedCount{ 0 };
    std::atomic<bool> closedFlag{ false };
    std::vector<std::string> receivedMessages;

    void OnCreated(MuteNet::ALink* link) override {}

    void OnReceived(MuteNet::ALink* link, const char* data, size_t length) override {
        if (data && length > 0) {
            receivedMessages.emplace_back(data, length);
            receivedCount++;
        }
    }

    void OnRemoteClosed(MuteNet::ALink* link) override {
        closedFlag = true;
    }

    void OnTLSHandshakeCompleted(MuteNet::ALink* link) override {}

    void OnError(MuteNet::ALink* link, int errorCode, const std::string& errorMsg) override {}
};


// 2. TCP Acceptor 및 TCPLinkImpl 연동 테스트 (IODispatcher 전달 및 shared_ptr 적용)
TEST_F(NetworkTest, TcpAcceptorAndMultipleDataExchangeTest) {

    std::shared_ptr<MuteNet::IODispatcher> dispatcher = std::make_shared<MuteNet::IODispatcher>();
    dispatcher->Init();

    auto acceptor = std::make_shared<MuteNet::Acceptor>(dispatcher);

    std::atomic<bool> isAccepted{ false };
    std::shared_ptr<MuteNet::Socket> acceptedClientSocket = nullptr;

    acceptor->SetCallbacks(
        [&](std::shared_ptr<MuteNet::Socket> clientSocket) {
            if (clientSocket != nullptr) {
                acceptedClientSocket = clientSocket;
                isAccepted = true;
            }
        },
        [](int errorCode, const std::string& errorMsg) {}
    );

    std::string testIp = "127.0.0.1";
    uint16_t testPort = 19997;

    bool listenSuccess = acceptor->Listen(testIp, testPort);
    ASSERT_TRUE(listenSuccess);

    auto clientCallbacks = std::make_shared<TestLinkCallbacks>();
    auto tcpClient = std::make_shared<MuteNet::TCPLinkImpl>(clientCallbacks, dispatcher);
    ASSERT_TRUE(tcpClient->GetSocket()->Connect(testIp, testPort));

    const int tcpSendIteration = 5;
    for (int i = 0; i < tcpSendIteration; ++i) {
        std::string tcpMessage = "TCP Multi-Send Message #" + std::to_string(i + 1);
        bool tcpSendResult = tcpClient->Send(tcpMessage.c_str(), tcpMessage.length());
        EXPECT_TRUE(tcpSendResult);
    }

    acceptor->Close();
}
void NetworkTest::SetUp()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
}

void NetworkTest::TearDown()
{
}
