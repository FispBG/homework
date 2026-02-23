cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 20)

project(homework)

add_executable(homework sources/main.cpp
        includes/functions.h
        sources/functions.cpp
)