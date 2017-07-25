cmake_minimum_required(VERSION 3.5)
project(priority_reader_writer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp)

find_package(Threads)
add_executable(priority_reader_writer ${SOURCE_FILES})
target_link_libraries(priority_reader_writer ${CMAKE_THREAD_LIBS_INIT})
