
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    ADD_DEFINITIONS(-DIOCONTEXT_KQUEUE)
    ADD_DEFINITIONS(-DAPPLE)
    INCLUDE_DIRECTORIES(/usr/local/include)
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    ADD_DEFINITIONS(-DIOCONTEXT_EPOLL)
    ADD_DEFINITIONS(-DLINUX)
ELSEIF(WIN32)
    ADD_DEFINITIONS(-DIOCONTEXT_IOCP)
ENDIF()

IF(NOT WIN32)
    ADD_SUBDIRECTORY(../lib/tbb)
    target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ../lib/tbb/include)

    target_link_libraries(${PROJECT_NAME} PRIVATE tbb)
ENDIF()

ADD_LIBRARY(
        ${CMAKE_PROJECT_NAME}
        IoContextEvent.cpp IoContextEvent.hpp
        IoContextThreadPool.cpp IoContextThreadPool.hpp
        EventBaseComponent.hpp
        TypeDefine.hpp
        IoContextImpl.cpp IoContextImpl.hpp
        ListenerComponent.cpp ListenerComponent.hpp
        Win32ListenerComponent.cpp Win32ListenerComponent.hpp
        LikeUnixListenerComponent.cpp LikeUnixListenerComponent.h
        Descriptor.cpp Descriptor.h
        SelectIOContext.cpp SelectIOContext.hpp
        WinDescriptor.cpp WinDescriptor.hpp
        IocpContextImpl.cpp IocpContextImpl.hpp
        UnixLikeDescriptor.cpp UnixLikeDescriptor.hpp
        UnixLikeIOContextImpl.hpp KQueueIOContextImpl.cpp
        KQueueIOContextImpl.hpp UnixLikeIOContextImpl.cpp
        EpollContextImpl.cpp EpollContextImpl.hpp
        ServerHandle.hpp ServerHandleImpl.cpp
        ServerHandleImpl.hpp NetworkHelpers.hpp
        NetworkHelpers.cpp TCPLink.hpp TCPLink.cpp
        TCPLinkImpl.cpp TCPLinkImpl.hpp
        EventBaseComponent.cpp
        InputMemoryStream.cpp InputMemoryStream.h
        MemoryStream.cpp MemoryStream.h
        OutputMemoryStream.cpp OutputMemoryStream.h
        Task.cpp Task.h
        TaskAgent.cpp TaskAgent.h
        TaskManager.cpp TaskManager.h
        MacroFunction.h Singleton.h
        CircularBuffer.cpp CircularBuffer.h SharedPool.hpp Runnable.hpp Event.hpp Event.cpp AtomicCounter.hpp)
