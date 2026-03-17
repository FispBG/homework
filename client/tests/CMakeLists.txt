cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 20)

project(test)

include(FetchContent)

FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.x)

FetchContent_MakeAvailable(googletest)

add_executable(${PROJECT_NAME} test_input.cpp
        ../../commonFunc/sources/functions.cpp
        ../sources/AppSettings.cpp
        ../sources/Tests.cpp
        ../sources/SocketClient.cpp
        ../includes/Tests.h
        ../includes/AppSettings.h
        ../includes/SocketClient.h
        ../../commonFunc/includes/functions.h
)

target_link_libraries(${PROJECT_NAME} gtest_main gmock_main)

