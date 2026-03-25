cmake_minimum_required(VERSION 3.27)
project(server)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(json)

add_executable(${PROJECT_NAME}
        sources/main.cpp
        sources/SocketServer.cpp
        sources/MathFunc.cpp
        sources/ManageServer.cpp
        sources/EpollServer.cpp
        sources/ThreadPool.cpp
        ../commonFunc/sources/functions.cpp

        includes/SocketServer.h
        includes/EpollServer.h
        includes/ManageServer.h
        includes/MathFunc.h
        includes/ThreadPool.h
        ../commonFunc/includes/functions.h
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/includes
        ${CMAKE_CURRENT_SOURCE_DIR}/../commonFunc/includes
)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

include(cmake/cppcheck.cmake)
