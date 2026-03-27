cmake_minimum_required(VERSION 3.27)
project(server)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

add_executable(${PROJECT_NAME}
        sources/main.cpp
        sources/SocketServer.cpp
        sources/MathFunc.cpp
        sources/ManageServer.cpp
        sources/EpollServer.cpp
        sources/ThreadPool.cpp
        sources/ConfigApp.cpp
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
        includes/SocketServer.h
        includes/EpollServer.h
        includes/ManageServer.h
        includes/MathFunc.h
        includes/ThreadPool.h
        includes/ConfigApp.h
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/includes
        ${CMAKE_CURRENT_SOURCE_DIR}/../commonFunc/includes
)

include(cmake/cppcheck.cmake)
