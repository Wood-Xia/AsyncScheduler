cmake_minimum_required(VERSION 3.3)
#project(gmockgtest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(gmockgtest STATIC gmock-gtest-all.cc)