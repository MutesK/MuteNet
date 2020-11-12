cmake_minimum_required(VERSION 3.15)
SET(PROJECT_NAME MuteNet)
SET(CMAKE_CXX_STANDARD 17)

SET(SRC_FILES Acceptor.cpp Acceptor.h
        HostNameLookup.cpp HostNameLookup.h
        IPLookUp.cpp IPLookUp.h
        Link.h LinkImpl.cpp LinkImpl.h
        Listener.cpp
        Network.cpp Network.h
        NetworkManager.cpp NetworkManager.h
        ServerHandle.h
        ServerHandleImpl.h ServerHandleImpl.cpp
        IoContextEvent.cpp IoContextEvent.hpp IoContextThreadPool.cpp IoContextThreadPool.hpp EventBaseComponent.cpp EventBaseComponent.hpp TypeDefine.hpp IoContextImpl.cpp IoContextImpl.hpp ListenerComponent.cpp ListenerComponent.hpp Win32ListenerComponent.cpp Win32ListenerComponent.h LikeUnixListenerComponent.cpp LikeUnixListenerComponent.h)


SET(CMAKE_VERBOSE_MAKEFILE TRUE)
SET(MUTENET_FOUNDATION_LOCATION ../Foundation)

PROJECT(${PROJECT_NAME})
SET(PROJECT_VERSION_MAJOR 1)
SET(PROJECT_VERSION_MINOR 0)

IF (UNIX)
    INCLUDE_DIRECTORIES(/usr/local/include)
ENDIF ()


ADD_SUBDIRECTORY(${MUTENET_FOUNDATION_LOCATION} ../Build)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${TBB_INCLUDE_DIRS} ${MUTENET_FOUNDATION_LOCATION})

# 출력 디렉토리
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../Build)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../Build)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../Build)

ADD_LIBRARY(${PROJECT_NAME} ${SRC_FILES})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${TBB_LIBRARIES} ${MUTENET_FOUNDATION_LOCATION}/Build)