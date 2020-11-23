cmake_minimum_required(VERSION 3.15)
SET(PROJECT_NAME MuteNetFoundation)
SET(CMAKE_CXX_STANDARD 17)

SET(SRC_FILES
        InputMemoryStream.cpp InputMemoryStream.h
        Logger.cpp Logger.h
        MemoryStream.cpp MemoryStream.h
        OutputMemoryStream.cpp OutputMemoryStream.h
        Task.cpp Task.h
        TaskAgent.cpp TaskAgent.h
        TaskManager.cpp TaskManager.h
        MacroFunction.h Singleton.h
        CircularBuffer.cpp CircularBuffer.h SharedPool.hpp Runnable.cpp Runnable.hpp Event.hpp Event.cpp AtomicCounter.hpp)


SET(CMAKE_VERBOSE_MAKEFILE TRUE)

PROJECT(${PROJECT_NAME})
SET(PROJECT_VERSION_MAJOR 1)
SET(PROJECT_VERSION_MINOR 0)

IF (NOT WIN32)
    INCLUDE_DIRECTORIES(/usr/local/include)
    FIND_PACKAGE(TBB REQUIRED)
ENDIF ()

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${TBB_INCLUDE_DIRS})

# 출력 디렉토리
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../Build)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../Build)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../Build)

ADD_LIBRARY(${PROJECT_NAME} ${SRC_FILES})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${TBB_LIBRARIES})
