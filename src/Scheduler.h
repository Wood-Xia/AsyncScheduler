//
// Created by gxdw68 on 2015/11/5.
//

#ifndef CODELAB_SCHEDULER_H
#define CODELAB_SCHEDULER_H

#include <functional>
#include <memory>
#include <cstddef>
#include <thread>

class Scheduler
{
public:
    typedef std::function<void()> Task;

    Scheduler();
    virtual ~Scheduler();
    Scheduler(const Scheduler&) = delete;
    const Scheduler& operator=(const Scheduler&) = delete;

    void start(const size_t workers = std::thread::hardware_concurrency());
    void stop();
    bool schedule(const Task& task);
    size_t workers() const;

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl_;
};


#endif //CODELAB_SCHEDULER_H
