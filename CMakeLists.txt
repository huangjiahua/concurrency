cmake_minimum_required(VERSION 3.12)
project(concurrency)

set(CMAKE_CXX_STANDARD 11)

add_executable(concurrency main.cpp ParallelTestSuite.h)
add_executable(queue_test2 queue_test.cpp ts_queue.h)

message("Flag: " ${CMAKE_CXX_FLAGS_RELEASE})
add_executable(parallel_test parallel_test.cpp ParallelTestSuite.h)

