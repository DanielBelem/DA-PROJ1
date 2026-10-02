cmake_minimum_required(VERSION 3.26.4)
project(DAPROJ1)

set(CMAKE_CXX_STANDARD 20)

add_executable(DAPROJ1
        data_structs/main.cpp
        data_structs/Reader.cpp
        data_structs/menu.cpp
        data_structs/menu.h
        data_structs/batcher.h
        data_structs/batcher.cpp
        data_structs/exercises.cpp
        data_structs/exercises.h
)
