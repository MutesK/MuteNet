#include "Common.h"
#include "StreamBufferTest.h"

#include <Allocator.h>
#include <StreamBuffer.h>

using namespace MuteNet;


void StreamBufferTest::SetUp()
{
}

void StreamBufferTest::TearDown()
{
}



TEST(StreamBufferTest, BasicWriteReadPeek) {
    StreamBuffer buf(64);
    Byte sendData[] = "Hello MuteNet Buffer!";
    size_t len = sizeof(sendData);

    buf.Write(sendData, len);

    Byte peekData[128] = { 0 };
    EXPECT_TRUE(buf.Peek(peekData, len));
    EXPECT_STREQ(reinterpret_cast<char*>(peekData), reinterpret_cast<char*>(sendData));

    Byte recvData[128] = { 0 };
    EXPECT_TRUE(buf.Read(recvData, len));
    EXPECT_STREQ(reinterpret_cast<char*>(recvData), reinterpret_cast<char*>(sendData));
}

TEST(StreamBufferTest, WrapAroundAndCapacityExpansion) {
    StreamBuffer buf(16);
    Byte dummyWrite[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    Byte dummyRead[10] = { 0 };

    buf.Write(dummyWrite, 8);
    buf.Read(dummyRead, 5);

    Byte largeData[100];
    for (int i = 0; i < 100; ++i) largeData[i] = static_cast<Byte>(i % 256);

    buf.Write(largeData, 100);

    Byte readLarge[100] = { 0 };
    buf.Read(dummyRead, 3);

    EXPECT_TRUE(buf.Read(readLarge, 100));
    EXPECT_EQ(std::memcmp(largeData, readLarge, 100), 0);
}

TEST(StreamBufferTest, DISABLED_LongRunStressTest) {
    constexpr auto TEST_DURATION = std::chrono::hours(1);

    std::cout << "\n======================================================\n";
    std::cout << " [START] 1-Hour StreamBuffer Wave-Pattern Stress Test \n";
    std::cout << "======================================================\n\n";

    StreamBuffer* buffer = Allocator::New<StreamBuffer>(128);

    uint64_t totalBytesProcessed = 0;
    uint64_t totalWriteCount = 0;
    uint64_t expansionCount = 0;

    size_t peakDataSize = 0;

    std::mt19937 rng(1337);
    std::uniform_int_distribution<size_t> distWriteSize(64, 512);

    struct PacketBlock {
        std::vector<Byte> data;
    };
    std::deque<PacketBlock> pendingPackets;

    // 파도 패턴 제어 변수 (false: Accumulate Phase, true: Drain Phase)
    bool isDraining = false;

    auto startTime = std::chrono::steady_clock::now();
    auto lastReportTime = startTime;
    auto lastCheckTime = startTime;

    while (true) {
        // [파도 상한선 체크] 버퍼에 대략 32KB 이상 쌓이면 'Drain(비우기)' 모드로 전환
        if (!isDraining && buffer->GetSize() >= 32768) {
            isDraining = true;
        }
        // [파도 하한선 체크] 버퍼가 완벽히 비워지면 다시 'Accumulate(쌓기)' 모드로 전환
        else if (isDraining && pendingPackets.empty()) {
            isDraining = false;
        }

        // 1. Accumulate Phase: 쓰기 위주로 실행하여 Size 증가 & Reserve 유발
        if (!isDraining) {
            size_t writeLen = distWriteSize(rng);
            std::vector<Byte> pattern(writeLen);

            for (size_t i = 0; i < writeLen; ++i) {
                pattern[i] = static_cast<Byte>((i + totalWriteCount) % 256);
            }

            size_t capBefore = buffer->GetCapacity();

            buffer->Write(pattern.data(), writeLen);
            pendingPackets.push_back({ pattern });

            size_t capAfter = buffer->GetCapacity();
            if (capAfter > capBefore) {
                expansionCount++;
            }

            if (buffer->GetSize() > peakDataSize) {
                peakDataSize = buffer->GetSize();
            }

            totalWriteCount++;
        }
        // 2. Drain Phase: 버퍼에 남아있는 패킷을 연속으로 완벽히 꺼내어 0 B까지 비움
        else if (!pendingPackets.empty()) {
            const auto& expected = pendingPackets.front();
            size_t readLen = expected.data.size();
            std::vector<Byte> readBuf(readLen);

            // Peek & Read 정합성 검증
            bool peekRes = buffer->Peek(readBuf.data(), readLen);
            EXPECT_TRUE(peekRes);
            EXPECT_EQ(std::memcmp(expected.data.data(), readBuf.data(), readLen), 0);

            std::fill(readBuf.begin(), readBuf.end(), 0);

            bool readRes = buffer->Read(readBuf.data(), readLen);
            EXPECT_TRUE(readRes);
            EXPECT_EQ(std::memcmp(expected.data.data(), readBuf.data(), readLen), 0);

            totalBytesProcessed += readLen;
            pendingPackets.pop_front();
        }

        // 시간 체크 및 10초 모니터링 출력
        auto now = std::chrono::steady_clock::now();
        if (now - lastCheckTime >= std::chrono::seconds(1)) {
            lastCheckTime = now;

            auto elapsedTime = now - startTime;
            if (elapsedTime >= TEST_DURATION) {
                break;
            }

            if (now - lastReportTime >= std::chrono::seconds(10)) {
                auto totalSecs = std::chrono::duration_cast<std::chrono::seconds>(elapsedTime).count();
                auto elapsedMins = totalSecs / 60;
                auto elapsedSecs = totalSecs % 60;

                std::cout << "------------------------------------------------------\n";
                std::cout << "[Progress] " << elapsedMins << "m " << elapsedSecs << "s elapsed\n";
                std::cout << "  - Phase           : " << (isDraining ? "DRAINING (Clearing)" : "ACCUMULATING (Filling)") << "\n";
                std::cout << "  - Current Size    : " << buffer->GetSize() << " B\n";
                std::cout << "  - Peak Usage Size : " << peakDataSize << " B\n";
                std::cout << "  - Capacity        : " << buffer->GetCapacity() << " B\n";
                std::cout << "  - Expansion (Res) : " << expansionCount << " times expanded\n";
                std::cout << "  - Total Writes    : " << totalWriteCount << " ops\n";
                std::cout << "  - Data Processed  : " << (totalBytesProcessed / (1024 * 1024)) << " MB\n";
                std::cout << "------------------------------------------------------" << std::endl;

                lastReportTime = now;
            }
        }
    }

    Allocator::Delete(buffer);

    std::cout << "\n======================================================\n";
    std::cout << " [COMPLETED] 1-Hour Wave-Pattern Test Finished!       \n";
    std::cout << " Peak Buffer Usage        : " << peakDataSize << " Bytes\n";
    std::cout << " Total Expansions Triggered : " << expansionCount << "\n";
    std::cout << "======================================================\n\n";
}
