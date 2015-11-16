//
// Created by gxdw68 on 2015/11/14.
//

#include "gtest/gtest.h"
#include "BlockingQueue.h"
#include <functional>
#include <thread>

typedef std::function<void()> ValueType;

TEST(BlockingQueue, shouldDefaultConstructAndDeconstructWithNoError)
{
    {
        BlockingQueue<ValueType> blockingQueue;
        EXPECT_EQ(0, blockingQueue.size());
        EXPECT_TRUE(blockingQueue.empty());
    }

    SUCCEED();
}

void putSome(BlockingQueue<ValueType>& blockingQueue, const size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        blockingQueue.put([](){});
    }
}

void takeSome(BlockingQueue<ValueType>& blockingQueue, const size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        blockingQueue.take();
    }
}

TEST(BlockingQueue, putWithoutTakeOnSameThreadShouldHasExpectedSize)
{
    BlockingQueue<ValueType> blockingQueue;
    const size_t n = 100;
    for (size_t i = 0; i < n; i++)
    {
        blockingQueue.put([](){});
    }

    EXPECT_EQ(n, blockingQueue.size());
}


TEST(BlockingQueue, putThenTakeOnSameThreadShouldHasExpectedSize)
{
    BlockingQueue<ValueType> blockingQueue;
    const size_t putCount = 100;
    const size_t takeCount = 50;
    for (size_t i = 0; i < putCount; i++)
    {
        blockingQueue.put([](){});
        blockingQueue.take();
    }
    EXPECT_EQ(0, blockingQueue.size());

    putSome(blockingQueue, putCount);
    takeSome(blockingQueue, takeCount);
    EXPECT_EQ((putCount - takeCount), blockingQueue.size());
}

TEST(BlockingQueue, putThenTakeInSeperateThreadShouldHasExpectedSize)
{
    BlockingQueue<ValueType> blockingQueue;
    const size_t putCount = 100;
    const size_t takeCount = 100;
    std::thread putThd(putSome, std::ref(blockingQueue), putCount);
    std::thread takeThd(takeSome, std::ref(blockingQueue), takeCount);
    takeThd.join();
    putThd.join();

    EXPECT_EQ((putCount-takeCount), blockingQueue.size());
}

TEST(BlockingQueue, takeThenPutInSeperateThreadShouldHasExpectedSize)
{
    BlockingQueue<ValueType> blockingQueue;
    const size_t putCount = 100;
    const size_t takeCount = 50;
    std::thread takeThd(takeSome, std::ref(blockingQueue), takeCount);
    std::thread putThd(putSome, std::ref(blockingQueue), putCount);

    takeThd.join();
    putThd.join();

    EXPECT_EQ((putCount-takeCount), blockingQueue.size());
}