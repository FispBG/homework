cmake_minimum_required(VERSION 3.27)
project(server)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
        sources/main.cpp
        sources/SocketServer.cpp
        sources/MathFunc.cpp
        sources/ManageServer.cpp
        sources/EpollServer.cpp
        ../commonFunc/sources/functions.cpp

        includes/SocketServer.h
        includes/EpollServer.h
        includes/ManageServer.h
        includes/MathFunc.h
        ../commonFunc/includes/functions.h
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/includes
        ${CMAKE_CURRENT_SOURCE_DIR}/../commonFunc/includes
)

include(cmake/cppcheck.cmake)