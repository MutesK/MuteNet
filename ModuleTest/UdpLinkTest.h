#include <gtest/gtest.h>
#include <UDPLinkImpl.h>
#include <thread>
#include <chrono>

using namespace MuteNet;

class UdpLinkTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(UdpLinkTest, SendAndReceiveData) {
    UDPLinkImpl serverLink(nullptr, nullptr);
    UDPLinkImpl clientLink(nullptr, nullptr);

    ASSERT_TRUE(serverLink.Open(9001));
    ASSERT_TRUE(clientLink.Open(0));

    const char* sendData = "Test Udp Message";
    bool received = false;
    char recvBuffer[256] = { 0 };

    std::thread receiverThread([&]() {
        char tempBuffer[256] = { 0 };
        std::string remoteIp;
        uint16_t remotePort = 0;

        int bytes = serverLink.ReceiveFrom(tempBuffer, sizeof(tempBuffer), remoteIp, remotePort);
        if (bytes > 0) {
            memcpy(recvBuffer, tempBuffer, bytes);
            received = true;
        }
        });

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    int sentBytes = clientLink.SendTo("127.0.0.1", 9001, sendData, strlen(sendData));
    EXPECT_GT(sentBytes, 0);

    if (receiverThread.joinable()) {
        receiverThread.join();
    }

    EXPECT_TRUE(received);
    EXPECT_STREQ(sendData, recvBuffer);

    serverLink.Close();
    clientLink.Close();
}