cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
project(homework)

add_executable(${PROJECT_NAME} sources/main.cpp
        includes/functions.h
        sources/functions.cpp
        includes/AppSettings.h
        sources/AppSettings.cpp
        includes/DataPool.h
        sources/DataPool.cpp
        sources/Menu.cpp
        includes/Menu.h
        includes/Tests.h
        sources/Tests.cpp
        includes/NetWork.h
        sources/NetWork.cpp
)
include(cmake/cppcheck.cmake)
