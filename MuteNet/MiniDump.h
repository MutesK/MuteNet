#pragma once

#include <string>
#include <memory>

namespace google_breakpad {
    class ExceptionHandler;
#if defined(__linux__)
    class MinidumpDescriptor; // Linux 전용 전방 선언
#endif
}

namespace MuteNet
{
    class MiniDump final
    {
    public:
        MiniDump();
        ~MiniDump();

        static void SetDumpPath(const std::string& path);
        static void Crash();

    private:
#if defined(_WIN32)
        static bool FilterCallback(void* context, struct _EXCEPTION_POINTERS* exinfo, void* assertion);
#elif defined(__APPLE__)
        static bool FilterCallback(void* context, void* exinfo, void* assertion);
        static bool MinidumpCallback(const char* dump_path, const char* minidump_id, void* context, bool succeeded);
#elif defined(__linux__)
        static bool FilterCallback(void* context);
        static bool MinidumpCallback(const google_breakpad::MinidumpDescriptor& descriptor, void* context, bool succeeded);
#endif

    private:
        static inline std::string s_dumpPath = ".";
        std::unique_ptr<google_breakpad::ExceptionHandler> m_handler;
    };
}