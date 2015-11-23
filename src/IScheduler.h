//
// Created by gxdw68 on 2015/11/19.
//

#ifndef ASYNCSCHEDULER_ISCHEDULER_H
#define ASYNCSCHEDULER_ISCHEDULER_H

#include <functional>
#include <memory>

class IScheduler
{
public:
    typedef std::function<void()> Task;

    virtual bool schedule(Task task) = 0;

    virtual size_t notCompletedTask() const = 0;
};

typedef std::shared_ptr<IScheduler> SchedulerPtr;

#endif //ASYNCSCHEDULER_ISCHEDULER_H
