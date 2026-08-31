#include <gtest/gtest.h>

#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif

int main(int argc, char** argv) {
#if defined(_WIN32) || defined(_WIN64)
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}