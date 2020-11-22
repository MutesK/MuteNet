cmake_minimum_required(VERSION 3.15)
SET(PROJECT_NAME MuteNet)
SET(CMAKE_CXX_STANDARD 17)

SET(SRC_FILES
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
        UnixLikeIOContextImpl.hpp KQueueIOContextImpl.cpp KQueueIOContextImpl.hpp UnixLikeIOContextImpl.cpp EpollContextImpl.cpp EpollContextImpl.hpp ServerHandle.hpp ServerHandleImpl.cpp ServerHandleImpl.hpp NetworkHelpers.hpp NetworkHelpers.cpp TCPLink.hpp TCPLink.cpp TCPLinkImpl.cpp TCPLinkImpl.hpp EventBaseComponent.cpp)


SET(CMAKE_VERBOSE_MAKEFILE TRUE)
SET(MUTENET_FOUNDATION_LOCATION ../Foundation)

PROJECT(${PROJECT_NAME})
SET(PROJECT_VERSION_MAJOR 1)
SET(PROJECT_VERSION_MINOR 0)


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


ADD_SUBDIRECTORY(${MUTENET_FOUNDATION_LOCATION} ../Build)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${TBB_INCLUDE_DIRS} ${MUTENET_FOUNDATION_LOCATION})

# 출력 디렉토리
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../Build)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../Build)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../Build)

ADD_LIBRARY(${PROJECT_NAME} ${SRC_FILES})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${TBB_LIBRARIES} ${MUTENET_FOUNDATION_LOCATION}/Build)
