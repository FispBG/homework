cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
project(client)

add_executable(${PROJECT_NAME}
        sources/main.cpp
        sources/Menu.cpp
        sources/AppSettings.cpp
        sources/Tests.cpp
        sources/NetWork.cpp
        sources/SocketClient.cpp
        ../commonFunc/sources/functions.cpp
        ../commonFunc/includes/functions.h

        ../commonFunc/includes/StandardPackets.h
        includes/AppSettings.h
        includes/DataPool.h
        includes/Menu.h
        includes/Tests.h
        includes/NetWork.h
        includes/SocketClient.h
)

include(cmake/cppcheck.cmake)
