#include <gtest/gtest.h>
#include <Acceptor.h>
#include <TCPLinkImpl.h>
#include <thread>
#include <chrono>
#include <atomic>

using namespace MuteNet;

class TcpNetworkTest : public ::testing::Test {
protected:
    const std::string TEST_IP = "127.0.0.1";
    uint16_t TEST_PORT = 12346;
};

TEST_F(TcpNetworkTest, AcceptAndDataExchange) {
    Acceptor acceptor;
    std::atomic<bool> clientAccepted{ false };
    std::atomic<bool> dataReceived{ false };
    char recvBuffer[256] = { 0 };

    acceptor.SetAcceptCallback([&](Tcplink* newLink) {
        if (newLink != nullptr) {
            clientAccepted = true;
            newLink->SetReceiveCallback([&](const char* data, size_t len) {
                memcpy(recvBuffer, data, len);
                dataReceived = true;
                });
        }
        });

    ASSERT_TRUE(acceptor.Start(TEST_IP, TEST_PORT));

    Tcplink clientLink;
    bool connected = clientLink.Connect(TEST_IP, TEST_PORT);
    ASSERT_TRUE(connected);

    auto startWait = std::chrono::steady_clock::now();
    while (!clientAccepted) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (std::chrono::steady_clock::now() - startWait > std::chrono::seconds(2)) break;
    }
    EXPECT_TRUE(clientAccepted);

    const char* message = "Hello TCP";
    clientLink.Send(message, strlen(message));

    startWait = std::chrono::steady_clock::now();
    while (!dataReceived) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (std::chrono::steady_clock::now() - startWait > std::chrono::seconds(2)) break;
    }

    EXPECT_TRUE(dataReceived);
    EXPECT_STREQ(message, recvBuffer);

    clientLink.Close();
    acceptor.Stop();
}