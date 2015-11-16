//
// Created by gxdw68 on 2015/11/7.
//

#include "gtest/gtest.h"
#include "Scheduler.h"

TEST(Scheduler, shouldDefaultConstructAndDeconstructWithNoError)
{
    {
        Scheduler scheduler;
    }

    SUCCEED();
}

TEST(Scheduler, startWithoutParameterShouldHaveDefautWorkers)
{
    Scheduler scheduler;
    auto workers = std::thread::hardware_concurrency();
    scheduler.start();
    EXPECT_EQ(workers, scheduler.workers());
}

TEST(Scheduler, startWithSpecifiedWorkersShouldHaveExpectedWorkers)
{
    Scheduler scheduler;
    const size_t workers = 2;
    scheduler.start(workers);
    EXPECT_EQ(workers, scheduler.workers());
}

TEST(Scheduler, stopWithoutStartShouldHaveZeroWorkers)
{
    Scheduler scheduler;
    scheduler.stop();
    EXPECT_EQ(0, scheduler.workers());
}

TEST(Scheduler, startThenStopShouldHaveZeroWorkers)
{
    Scheduler scheduler;
    scheduler.start();
    scheduler.stop();
    EXPECT_EQ(0, scheduler.workers());
}

TEST(Scheduler, scheduleAfterStartShouldSuccess)
{
    Scheduler scheduler;
    scheduler.start();
    EXPECT_TRUE(scheduler.schedule([](){}));
}

TEST(Scheduler, scheduleBeforeStartShouldFailure)
{
    Scheduler scheduler;
    EXPECT_FALSE(scheduler.schedule([](){}));
}