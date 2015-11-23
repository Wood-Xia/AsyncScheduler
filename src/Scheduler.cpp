//
// Created by gxdw68 on 2015/11/5.
//

#include "Scheduler.h"
#include "BlockingQueue.h"
#include <iostream>
#include <mutex>

struct Scheduler::Impl
{
    Impl() : isRunning_(false), isAborting_(false), taskCount_(0), runningThreads_(0)
    {

    }

    ~Impl()
    {
        stop();
    }

    void start(const size_t workers)
    {
        if(!isRunning_.load())
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
            isRunning_.store(true);
        }
    }

    void stop() noexcept
    {
        if(isRunning_.load())
        {
            while(runningThreads_.load() > 0)
            {
                abortPossibleWaitings();
            }
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

    bool schedule(Scheduler::Task task)
    {
        std::cout << "Impl scheduling task" << std::endl;
        // Policy? Chance balance or load balance?
        if (isRunning_.load() && !isAborting_.load())
        {
            std::lock_guard<std::mutex> lock(taskQueuesMutex_);
            auto which = taskCount_.load() % threads_.size();
            if (which <= taskQueues_.size() -1)
            {
                taskQueues_[which]->put(task);
                taskCount_.fetch_add(1);
                std::cout << "Impl scheduled task on worker#" << which << std::endl;
                return true;
            }
        }
        return false;
    }

    size_t workers() const
    {
        return threads_.size();
    }

    size_t notCompletedTask() const
    {
        return taskCount_.load();
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
        isAborting_.store(false);
    }

    void abortPossibleWaitings()
    {
        isAborting_.store(true);
        for (auto& que : taskQueues_)
        {
            que->abort();
        }
    }

    void workerHandler(const size_t which)
    {
        runningThreads_.fetch_add(1);
        while (!isAborting_.load())
        {
            std::cout << "worker thread#" << which << " waiting for task" << std::endl;
            auto task = taskQueues_[which]->take();
            if (task)
            {
                (*task)();
                std::cout << "executed on task at worker#" << which << std::endl;
                taskCount_.fetch_sub(1);
            }
        }
        runningThreads_.fetch_sub(1);
        std::cout << "worker thread#" << which << " aborted" << std::endl;
    }

private:
    std::atomic<bool> isRunning_;
    std::atomic<bool> isAborting_;
    std::atomic<size_t > taskCount_;
    std::atomic<size_t> runningThreads_;
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

bool Scheduler::schedule(Task task)
{
    std::cout << "scheduling task" << std::endl;
    return pImpl_->schedule(task);
}

size_t Scheduler::workers() const
{
    return pImpl_->workers();
}

size_t Scheduler::notCompletedTask() const
{
    return pImpl_->notCompletedTask();
}
