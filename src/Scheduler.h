//
// Created by gxdw68 on 2015/11/5.
//

#ifndef CODELAB_SCHEDULER_H
#define CODELAB_SCHEDULER_H

#include "IScheduler.h"
#include <functional>
#include <memory>
#include <cstddef>
#include <thread>

class Scheduler : public IScheduler
{
public:
    Scheduler();
    virtual ~Scheduler();
    Scheduler(const Scheduler&) = delete;
    const Scheduler& operator=(const Scheduler&) = delete;
    // @TODO support lazy create worker thread is necessory?
    void start(const size_t workers = std::thread::hardware_concurrency());
    void stop();
    bool schedule(Task task);
    size_t workers() const;
    size_t notCompletedTask() const;

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl_;
};


#endif //CODELAB_SCHEDULER_H
