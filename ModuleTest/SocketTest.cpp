#include "Common.h"
#include "SocketTest.h"

#include <Socket.h>

using namespace MuteNet;


// -----------------------------------------------------------------------------
// 1. 소켓 기본 동작 테스트 (Socket LifeCycle Test)
// -----------------------------------------------------------------------------
TEST(SocketTest, CreateAndSetOptions) {
    Socket sock;

    // 소켓 생성 검증
    EXPECT_TRUE(sock.Create());
    EXPECT_TRUE(sock.IsValid());
    EXPECT_NE(sock.GetHandle(), INVALID_SOCKET_HANDLE);

    // 옵션 설정 검증 (Non-blocking, ReuseAddr)
    EXPECT_TRUE(sock.SetReuseAddr(true));
    EXPECT_TRUE(sock.SetNonBlocking(true));

    sock.Close();
    EXPECT_FALSE(sock.IsValid());
    EXPECT_EQ(sock.GetHandle(), INVALID_SOCKET_HANDLE);
}

TEST(SocketTest, BindAndListen) {
    Socket serverSock;

    ASSERT_TRUE(serverSock.Create());
    EXPECT_TRUE(serverSock.SetReuseAddr(true));

    // 포트 0으로 바인딩 테스트
    uint16_t testPort = 0;
    EXPECT_TRUE(serverSock.Bind("127.0.0.1", testPort));
    EXPECT_TRUE(serverSock.Listen(10));

    serverSock.Close();
}