#include "MiniDumpTest.h"

#include <fstream>
#include <string>
#include <thread>  
#include <chrono>  

#include <MiniDump.h>
using namespace MuteNet;

void MiniDumpTest::SetUp()
{
    testDumpDir = fs::absolute("./test_dumps");

    if (fs::exists(testDumpDir))
    {
        fs::remove_all(testDumpDir);
    }
    fs::create_directories(testDumpDir);
}

void MiniDumpTest::TearDown()
{
	if (fs::exists(testDumpDir))
	{
		fs::remove_all(testDumpDir);
	}
}

bool MiniDumpTest::HasDumpFileBeenCreated() const
{
    if (!fs::exists(testDumpDir))
        return false;

    // 파일 쓰기 동기화 대기
    for (int i = 0; i < 15; ++i)
    {
        for (const auto& entry : fs::directory_iterator(testDumpDir))
        {
            if (entry.is_regular_file() && entry.path().extension() == ".dmp")
            {
                if (fs::file_size(entry.path()) > 0)
                {
                    return true;
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return false;
}

TEST_F(MiniDumpTest, SetDumpPathAndInit)
{
    MuteNet::MiniDump::SetDumpPath(testDumpDir.string());

    EXPECT_NO_THROW({
        MuteNet::MiniDump dumpHandler;
        });
}

/*
TEST_F(MiniDumpTest, CrashAndGenerateDumpFile)
{
    std::string dumpPathStr = testDumpDir.string();

    // Death Test 매크로 실행
    ASSERT_DEATH({
        MuteNet::MiniDump::SetDumpPath(dumpPathStr);
        MuteNet::MiniDump dumpHandler;

        // 핸들러 등록 후 안정적인 크래시 호출
        MuteNet::MiniDump::Crash();
        }, ".*");

    EXPECT_TRUE(HasDumpFileBeenCreated()) << "크래시 발생 후 .dmp 파일이 지정된 경로(" << dumpPathStr << ")에 생성되지 않았습니다.";
}
*/