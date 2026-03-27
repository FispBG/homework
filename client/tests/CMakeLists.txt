cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 20)

project(test)

include(FetchContent)

FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.x)

FetchContent_MakeAvailable(googletest)

add_executable(${PROJECT_NAME}
        AppSettingsTest.cpp
        CheckAppTest.cpp
        DataPoolTets.cpp
        FunctionTest.cpp

        ../sources/AppSettings.cpp
        ../sources/Tests.cpp
        ../sources/SocketClient.cpp
        ../sources/NetWork.cpp
        ../../commonFunc/sources/ResultStatus.cpp
        ../../commonFunc/sources/PacketFunction.cpp
        ../../commonFunc/sources/StringFunction.cpp
        ../../commonFunc/sources/ValidationFunction.cpp
        ../../commonFunc/sources/VectorProcess.cpp

        ../../commonFunc/includes/ResultStatus.h
        ../../commonFunc/includes/PacketFunction.h
        ../../commonFunc/includes/StringFunction.h
        ../../commonFunc/includes/ValidationFunction.h
        ../../commonFunc/includes/VectorProcess.h
        ../../commonFunc/includes/StandardPackets.h
        ../includes/Tests.h
        ../includes/AppSettings.h
        ../includes/SocketClient.h
        ../includes/NetWork.h
)

target_link_libraries(${PROJECT_NAME} gtest_main gmock_main)

