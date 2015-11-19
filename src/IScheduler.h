//
// Created by gxdw68 on 2015/11/19.
//

#ifndef ASYNCSCHEDULER_ISCHEDULER_H
#define ASYNCSCHEDULER_ISCHEDULER_H

#include <functional>

class IScheduler
{
public:
    typedef std::function<void()> Task;

    virtual bool schedule(const Task& task) = 0;
};
#endif //ASYNCSCHEDULER_ISCHEDULER_H
