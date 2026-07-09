cmake_minimum_required(VERSION 3.20)
project(MiniSQL CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(minisql 
src/main.cpp 
src/storage/disk_manager.cpp
src/catalog/row.cpp
)
target_include_directories(minisql PRIVATE include)
