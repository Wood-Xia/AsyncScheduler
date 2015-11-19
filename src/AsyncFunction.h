//
// Created by gxdw68 on 2015/11/19.
//

#ifndef ASYNCSCHEDULER_ASYNCFUNCTION_H
#define ASYNCSCHEDULER_ASYNCFUNCTION_H

#include <functional>
#include <memory>
#include "IScheduler.h"

template<typename... ARGS>
class AsyncFunction
{
public:
    AsyncFunction(std::shared_ptr<IScheduler> scheduler, std::function<void(ARGS...)> f )
            : function_(f) {}

    void operator()(ARGS... args)  { scheduler_->schedule([]() {function_(args...);});  }

private:
    std::function<void(ARGS...)> function_;
    std::shared_ptr<IScheduler> scheduler_;
};

#endif //ASYNCSCHEDULER_ASYNCFUNCTION_H
