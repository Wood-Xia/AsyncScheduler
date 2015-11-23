//
// Created by gxdw68 on 2015/11/11.
//

#ifndef CODELAB_BLOCKINGQUEUE_H
#define CODELAB_BLOCKINGQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <experimental/optional>
#include <iostream>

template <typename T>
class BlockingQueue
{
public:
    typedef T ValueType;
    BlockingQueue() : aborted_(false)
    {
    };

    virtual ~BlockingQueue()
    {
        if(!aborted_)
        {
            abort();
        }
    };

    void put(const ValueType& e)
    {
        {
            std::lock_guard<std::mutex> lock(queueAccessMutex_);
            queue_.push(e);
        }
        queueDataReadyCv_.notify_one();
    }

    void put(ValueType&& e)
    {
        {
            std::lock_guard<std::mutex> lock(queueAccessMutex_);
            queue_.push(std::forward<T>(e));
        }
        queueDataReadyCv_.notify_one();
    }

    std::experimental::optional<ValueType> take()
    {
        std::unique_lock<std::mutex> lock(queueAccessMutex_);
        queueDataReadyCv_.wait(lock, [this]()
            {
                auto isNotEmpty = !queue_.empty();
                return (isNotEmpty || aborted_);
            }
        );
        if(!queue_.empty())
        {
            auto value = std::experimental::optional<ValueType>(queue_.front());
            queue_.pop();
            return value;
        }
        else
        {
            return std::experimental::optional<ValueType>();
        }

    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(queueAccessMutex_);
        return queue_.empty();
    }

    size_t size() const
    {
        std::lock_guard<std::mutex> lock(queueAccessMutex_);
        return queue_.size();
    }

    void abort()
    {
        aborted_ = true;
        queueDataReadyCv_.notify_all();
    }

private:
    std::queue<T> queue_;
    mutable std::mutex queueAccessMutex_;
    std::condition_variable queueDataReadyCv_;
    std::atomic<bool> aborted_;
};
#endif //CODELAB_BLOCKINGQUEUE_H
