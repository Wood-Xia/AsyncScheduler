cmake_minimum_required(VERSION 3.3)
project(AsyncScheduler)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# in case of using Cygwin, use the GNU dialect of the C++ language to integrate with Google Test
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")

set(GMOCK_GTEST_DIR libs)

include_directories(src)
include_directories(libs)

add_subdirectory(libs bin/libs)
add_subdirectory(src bin/src)
add_subdirectory(test bin/test)
add_subdirectory(demo bin/demo)

