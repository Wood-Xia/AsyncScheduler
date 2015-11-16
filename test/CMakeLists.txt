cmake_minimum_required(VERSION 3.3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++1y")

set(TEST_CASES SchedulerTest.cpp FunctorWrapperTest.cpp BlockingQueueTest.cpp)

add_executable(RunTestCases TestMain.cpp ${TEST_CASES})
target_link_libraries(RunTestCases gmockgtest scheduler)
