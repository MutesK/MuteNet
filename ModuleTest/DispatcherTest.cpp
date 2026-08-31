#include "Common.h"
#include "DispatcherTest.h"

void DispatcherTest::SetUp()
{
    ASSERT_TRUE(dispatcher.Init(2));

    ASSERT_TRUE(serverSock.Create());
    ASSERT_TRUE(serverSock.SetReuseAddr(true));
    ASSERT_TRUE(serverSock.SetNonBlocking(true));

    // 0으로 Bind하여 OS가 사용 가능한 동적 포트를 자동 할당하도록 함
    ASSERT_TRUE(serverSock.Bind("127.0.0.1", 0));
    ASSERT_TRUE(serverSock.Listen(5));

    // OS가 실제로 바인딩한 포트를 가져옴 (Socket에 GetLocalPort 메서드가 없을 경우 대비한 C-Style 코드)
    sockaddr_in sin{};
    socklen_t len = sizeof(sin);
    if (::getsockname(serverSock.GetHandle(), reinterpret_cast<sockaddr*>(&sin), &len) == 0) {
        port = ntohs(sin.sin_port);
    }

    ASSERT_TRUE(dispatcher.RegisterSocket(serverSock.GetHandle(), &serverSock));
}

void DispatcherTest::TearDown()
{
    acceptedSock.Close();
    clientSock.Close();
    serverSock.Close();
    dispatcher.Stop();
}


// -----------------------------------------------------------------------------
// 2. I/O 디스패처 초기화 및 등록 테스트 (Dispatcher Init & Register Test)
// -----------------------------------------------------------------------------
TEST(DispatcherBasicTest, InitAndStop) {
    IODispatcher dispatcher;
    EXPECT_TRUE(dispatcher.Init(2));
    dispatcher.Stop();
}

TEST(DispatcherBasicTest, RegisterSocket) {
    IODispatcher dispatcher;
    ASSERT_TRUE(dispatcher.Init(2));

    Socket sock;
    ASSERT_TRUE(sock.Create());

    int dummyContext = 42;
    EXPECT_TRUE(dispatcher.RegisterSocket(sock.GetHandle(), &dummyContext));

    sock.Close();
    dispatcher.Stop();
}

TEST_F(DispatcherTest, AsyncSendAndReceive) {
    std::atomic<bool> readSuccess{ false };
    std::atomic<uint32_t> receivedBytes{ 0 };
    std::string receivedData;
    receivedData.resize(1024);

    // 1. 디스패처 비동기 이벤트 콜백 정의
    dispatcher.SetCallback([&](bool success, uint32_t bytes, IOOperation op, void* context) {
        if (!success) return;

        if (op == IOOperation::Read) {
            readSuccess = true;
            receivedBytes = bytes;
        }
        });

    // 2. 클라이언트 소켓 생성 및 서버 연결
    ASSERT_TRUE(clientSock.Create());
    ASSERT_TRUE(clientSock.Connect(ip, port));

    // 3. 서버 측 Accept 처리
    std::string clientIp;
    uint16_t clientPort = 0;
    SocketHandle acceptHandle = INVALID_SOCKET_HANDLE;

    // Non-blocking Accept 시도
    for (int i = 0; i < 10; ++i) {
        acceptHandle = serverSock.Accept(&clientIp, &clientPort);
        if (acceptHandle != INVALID_SOCKET_HANDLE) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    ASSERT_NE(acceptHandle, INVALID_SOCKET_HANDLE);

    // Accept된 소켓 디스패처에 등록
#if defined(_WIN32) || defined(_WIN64)
    OverlappedContext asyncCtx{};
    asyncCtx.Context = &acceptedSock;
#else
    EpollAsyncContext asyncCtx{}; // macOS의 경우 KqueueAsyncContext
    asyncCtx.Context = &acceptedSock;
#endif

    ASSERT_TRUE(dispatcher.RegisterSocket(acceptHandle, &asyncCtx));

    // 4. 서버 측 비동기 수신(Read) 대기 요청
    ASSERT_TRUE(dispatcher.PostAsyncRead(acceptHandle, &receivedData[0], receivedData.size(), &asyncCtx));

    // 5. 클라이언트에서 송신(Send)
    std::string sendMsg = "Hello Google Test Async!";
    int sent = clientSock.Send(sendMsg.c_str(), sendMsg.length());
    EXPECT_GT(sent, 0);

    // 6. Worker Thread가 비동기로 수신을 완료할 때까지 대기 (최대 2초)
    for (int i = 0; i < 200; ++i) {
        if (readSuccess) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // 7. 검증
    EXPECT_TRUE(readSuccess);
    EXPECT_EQ(receivedBytes, sendMsg.length());
    EXPECT_EQ(receivedData.substr(0, receivedBytes), sendMsg);
}