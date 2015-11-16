//
// Created by gxdw68 on 2015/11/5.
//

#include "Scheduler.h"
#include "BlockingQueue.h"
#include <iostream>
#include <mutex>

struct Scheduler::Impl
{
    Impl() : isRunning_(false), isAborting_(false), taskCount_(0)
    {

    }

    ~Impl()
    {
        stop();
    }

    void start(const size_t workers)
    {
        if(!isRunning_)
        {
            std::cout << "starting " << workers << " workers" << std::endl;
            threads_.reserve(workers);
            taskQueues_.reserve(workers);
            for (auto i = 0; i < workers; i++)
            {
                TaskQuePtr taskQue(new TaskQue);
                taskQueues_.push_back(std::move(taskQue));
                ThreadPtr thd(new std::thread(std::bind(&Impl::workerHandler, this, i)));
                threads_.push_back(std::move(thd));
            }
            isRunning_ = true;
        }

    }

    void stop() noexcept
    {
        if(isRunning_)
        {
            abortPossibleWaitings();
            waitWorkThreadsFinished();
            {
                std::lock_guard<std::mutex> lock(taskQueuesMutex_);
                taskQueues_.clear();
                taskCount_ = 0;
            }
            threads_.clear();
            isRunning_ = false;
        }
    }

    bool schedule(const Scheduler::Task& task)
    {
        // Policy? Chance balance or load balance?
        if (isRunning_ && !isAborting_)
        {
            std::lock_guard<std::mutex> lock(taskQueuesMutex_);
            auto n = taskCount_++;
            auto which = n % threads_.size();
            if (which <= taskQueues_.size() -1)
            {
                taskQueues_[which]->put(task);
                return true;
            }
        }
        return false;
    }

    size_t workers() const
    {
        return threads_.size();
    }

private:
    void waitWorkThreadsFinished()
    {
        for (auto& e : threads_)
        {
            try
            {
                e->join();
            }
            catch (const std::system_error&)
            {
                // ensure no excepption in deconstructor
            }
        }
        isAborting_ = false;
    }

    void abortPossibleWaitings()
    {
        isAborting_ = true;
        for (auto& que : taskQueues_)
        {
            que->abort();
        }
    }

    void workerHandler(const size_t which)
    {
        while (isRunning_ && !isAborting_)
        {
            auto task = taskQueues_[which]->take();
            if (task)
            {
                (*task)();
                --taskCount_;
            }
        }

    }

private:
    std::atomic<bool> isRunning_;
    std::atomic<bool> isAborting_;
    std::atomic<size_t > taskCount_;
    typedef std::unique_ptr<std::thread> ThreadPtr;
    std::vector<ThreadPtr> threads_;
    typedef BlockingQueue<Task> TaskQue;
    typedef std::unique_ptr<TaskQue> TaskQuePtr;
    std::vector<TaskQuePtr> taskQueues_;
    mutable std::mutex taskQueuesMutex_;
};



Scheduler::Scheduler() : pImpl_(new Impl())
{
}

Scheduler::~Scheduler()
{
}

void Scheduler::start(const size_t workers)
{
    pImpl_->start(workers);
}

void Scheduler::stop()
{
    pImpl_->stop();
}

bool Scheduler::schedule(const Task& task)
{
    return pImpl_->schedule(task);
}

size_t Scheduler::workers() const
{
    return pImpl_->workers();
}
