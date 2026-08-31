#include "MiniDump.h"
#include <iostream>

#if defined(_WIN32)
#include <windows.h>
#include <crtdbg.h>
#include "client/windows/handler/exception_handler.h"
#elif defined(__APPLE__)
#include "client/mac/handler/exception_handler.h"
#elif defined(__linux__)
#include "client/linux/handler/exception_handler.h"
#include "client/linux/handler/minidump_descriptor.h"
#endif

namespace MuteNet
{
#if defined(_WIN32)
    static bool MinidumpWriteCallback(
        const wchar_t* dump_path,
        const wchar_t* minidump_id,
        void* context,
        EXCEPTION_POINTERS* exinfo,
        MDRawAssertionInfo* assertion,
        bool succeeded)
    {
        return succeeded;
    }
#endif

    MiniDump::MiniDump()
    {
#if defined(_WIN32)
        _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
        _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);

        // 한글 경로 지원을 위한 안전한 UTF-8 -> UTF-16 변환
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, s_dumpPath.c_str(), (int)s_dumpPath.size(), NULL, 0);
        std::wstring wDumpPath(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, s_dumpPath.c_str(), (int)s_dumpPath.size(), &wDumpPath[0], size_needed);

        m_handler.reset(new google_breakpad::ExceptionHandler(
            wDumpPath,
            reinterpret_cast<google_breakpad::ExceptionHandler::FilterCallback>(FilterCallback),
            MinidumpWriteCallback,
            nullptr,
            google_breakpad::ExceptionHandler::HANDLER_ALL,
            MiniDumpNormal,
            static_cast<const wchar_t*>(nullptr),
            nullptr
        ));

#elif defined(__APPLE__)
        m_handler.reset(new google_breakpad::ExceptionHandler(
            s_dumpPath,
            FilterCallback,
            MinidumpCallback,
            nullptr,
            true,
            nullptr
        ));

#elif defined(__linux__)
        google_breakpad::MinidumpDescriptor descriptor(s_dumpPath);
        m_handler.reset(new google_breakpad::ExceptionHandler(
            descriptor,
            FilterCallback,
            MinidumpCallback,
            nullptr,
            true,
            -1
        ));
#endif
    }

    MiniDump::~MiniDump() = default;

    void MiniDump::SetDumpPath(const std::string& path)
    {
        s_dumpPath = path;
    }

    void MiniDump::Crash()
    {
        // 전 플랫폼 공통으로 컴파일러 최적화를 방지하여 확실하게 크래시 유발
        volatile int* nullPtr = nullptr;
        *nullPtr = 42;
    }

#if defined(_WIN32)
    bool MiniDump::FilterCallback(void* context, struct _EXCEPTION_POINTERS* exinfo, void* assertion)
    {
        return true;
    }
#elif defined(__APPLE__)
    bool MiniDump::FilterCallback(void* context, void* exinfo, void* assertion)
    {
        return true;
    }

    bool MiniDump::MinidumpCallback(const char* dump_path, const char* minidump_id, void* context, bool succeeded)
    {
        return succeeded;
    }
#elif defined(__linux__)
    bool MiniDump::FilterCallback(void* context)
    {
        return true;
    }

    bool MiniDump::MinidumpCallback(const google_breakpad::MinidumpDescriptor& descriptor, void* context, bool succeeded)
    {
        return succeeded;
    }
#endif
}