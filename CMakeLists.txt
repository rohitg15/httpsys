cmake_minimum_required(VERSION 3.6)
set (CMAKE_CXX_STANDARD 11)
set (CXX_DEFINES -std=c++14)

project(myProject)

add_subdirectory(main)
add_subdirectory(libthreads)
add_subdirectory(libhttpserver)
