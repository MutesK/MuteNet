#include "Common.h"
#include "AllocatorTest.h"
#include "Allocator.h"

using namespace MuteNet;


void AllocatorTest::SetUp()
{
}

void AllocatorTest::TearDown()
{
}



TEST(AllocatorTest, BasicNewDelete) {
    struct TestDummy {
        int a;
        double b;
        TestDummy(int _a, double _b) : a(_a), b(_b) {}
    };

    TestDummy* ptr = Allocator::New<TestDummy>(100, 3.14);
    ASSERT_NE(ptr, nullptr);
    EXPECT_EQ(ptr->a, 100);
    EXPECT_DOUBLE_EQ(ptr->b, 3.14);

    Allocator::Delete(ptr);
}


TEST(AllocatorHardTest, AlignmentAndBoundaryCheck) {
    struct alignas(8) Align8 { char dummy[7]; };
    struct alignas(16) Align16 { char dummy[15]; };
    struct alignas(32) Align32 { char dummy[31]; };
    struct alignas(64) Align64 { char dummy[63]; };

    auto* p8 = Allocator::New<Align8>();
    auto* p16 = Allocator::New<Align16>();
    auto* p32 = Allocator::New<Align32>();
    auto* p64 = Allocator::New<Align64>();

    EXPECT_EQ(reinterpret_cast<uintptr_t>(p8) % 8, 0u);
    EXPECT_EQ(reinterpret_cast<uintptr_t>(p16) % 16, 0u);
    EXPECT_EQ(reinterpret_cast<uintptr_t>(p32) % 32, 0u);
    EXPECT_EQ(reinterpret_cast<uintptr_t>(p64) % 64, 0u);

    Allocator::Delete(p8);
    Allocator::Delete(p16);
    Allocator::Delete(p32);
    Allocator::Delete(p64);

    auto& res = Allocator::GetResource();
    constexpr size_t testSizes[] = { 1, 2, 4, 8, 13, 31, 64, 128, 1024, 4096, 8192, 65536 };
    for (size_t size : testSizes) {
        void* ptr = res.allocate(size, 16);
        ASSERT_NE(ptr, nullptr);
        EXPECT_EQ(reinterpret_cast<uintptr_t>(ptr) % 16, 0u);
        std::memset(ptr, 0xFE, size);
        res.deallocate(ptr, size, 16);
    }
}

TEST(AllocatorHardTest, RandomAllocationAndDeallocationPattern) {
    constexpr size_t MAX_ALLOCS = 10000;
    constexpr size_t ITERATIONS = 100000;

    struct AllocInfo {
        void* ptr;
        size_t size;
        uint8_t pattern;
    };

    std::vector<AllocInfo> activeAllocations;
    activeAllocations.reserve(MAX_ALLOCS);

    std::mt19937 rng(42);
    std::uniform_int_distribution<size_t> distSize(1, 2048);
    std::uniform_int_distribution<int> distOp(0, 100);
    std::uniform_int_distribution<uint32_t> distPattern(1, 255);

    auto& res = Allocator::GetResource();

    for (size_t i = 0; i < ITERATIONS; ++i) {
        bool doAllocate = activeAllocations.empty() || (activeAllocations.size() < MAX_ALLOCS && distOp(rng) < 60);

        if (doAllocate) {
            size_t size = distSize(rng);
            uint8_t pattern = static_cast<uint8_t>(distPattern(rng));

            void* ptr = res.allocate(size, alignof(std::max_align_t));
            ASSERT_NE(ptr, nullptr);
            std::memset(ptr, pattern, size);

            activeAllocations.push_back({ ptr, size, pattern });
        }
        else {
            std::uniform_int_distribution<size_t> distIndex(0, activeAllocations.size() - 1);
            size_t idx = distIndex(rng);

            AllocInfo info = activeAllocations[idx];
            uint8_t* bytePtr = static_cast<uint8_t*>(info.ptr);
            for (size_t b = 0; b < info.size; ++b) {
                ASSERT_EQ(bytePtr[b], info.pattern);
            }

            res.deallocate(info.ptr, info.size, alignof(std::max_align_t));
            activeAllocations[idx] = activeAllocations.back();
            activeAllocations.pop_back();
        }
    }

    for (const auto& info : activeAllocations) {
        uint8_t* bytePtr = static_cast<uint8_t*>(info.ptr);
        for (size_t b = 0; b < info.size; ++b) {
            ASSERT_EQ(bytePtr[b], info.pattern);
        }
        res.deallocate(info.ptr, info.size, alignof(std::max_align_t));
    }
}

TEST(AllocatorHardTest, DISABLED_AllocatorLongRunStressTest) {
    constexpr auto TEST_DURATION = std::chrono::hours(1);

    std::cout << "\n======================================================\n";
    std::cout << " [START] 1-Hour Hardcore Single-Thread Allocator Test \n";
    std::cout << "======================================================\n\n";

    auto& res = Allocator::GetResource();
    std::mt19937 rng(777);

    uint64_t totalAllocOps = 0;
    uint64_t totalBytesAllocated = 0;

    struct MemChunk {
        void* ptr;
        size_t size;
    };
    std::vector<MemChunk> activeChunks;
    activeChunks.reserve(20000);

    auto startTime = std::chrono::steady_clock::now();
    auto lastReportTime = startTime;

    while (true) {
        std::uniform_int_distribution<size_t> distSize(8, 65536);
        std::uniform_int_distribution<int> distAction(0, 100);

        for (int i = 0; i < 5000; ++i) {
            if (activeChunks.empty() || (activeChunks.size() < 15000 && distAction(rng) < 70)) {
                size_t sz = distSize(rng);
                void* p = res.allocate(sz, 16);

                std::memset(p, 0xAB, std::min<size_t>(sz, 64));

                activeChunks.push_back({ p, sz });
                totalAllocOps++;
                totalBytesAllocated += sz;
            }
            else {
                size_t idx = rng() % activeChunks.size();
                res.deallocate(activeChunks[idx].ptr, activeChunks[idx].size, 16);

                activeChunks[idx] = activeChunks.back();
                activeChunks.pop_back();
            }
        }

        auto now = std::chrono::steady_clock::now();
        auto elapsedTime = now - startTime;

        if (elapsedTime >= TEST_DURATION) break;

        if (now - lastReportTime >= std::chrono::seconds(10)) {
            auto hours = std::chrono::duration_cast<std::chrono::hours>(elapsedTime).count();
            auto mins = std::chrono::duration_cast<std::chrono::minutes>(elapsedTime).count() % 60;

            std::cout << "[Progress " << hours << "h " << mins << "m] "
                << "Active Blocks: " << activeChunks.size() << ", "
                << "Total Alloc Ops: " << totalAllocOps << ", "
                << "Cumulated Alloc: " << (totalBytesAllocated / (1024 * 1024)) << " MB\n";

            lastReportTime = now;
        }
    }

    for (const auto& chunk : activeChunks) {
        res.deallocate(chunk.ptr, chunk.size, 16);
    }

    std::cout << "\n======================================================\n";
    std::cout << " [COMPLETED] 8-Hour Allocator Test Passed Successfully!\n";
    std::cout << "======================================================\n\n";
}