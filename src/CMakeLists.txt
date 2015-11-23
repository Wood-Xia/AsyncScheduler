cmake_minimum_required(VERSION 3.3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++1y")

set(HEADER_FILES
        IScheduler.h
        Scheduler.h
        BlockingQueue.h
        AsyncFunction.h
        )

set(SOURCE_FILES
        Scheduler.cpp
        )

add_library(scheduler STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(scheduler pthread)