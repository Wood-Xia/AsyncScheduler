//
// Created by gxdw68 on 2015/11/19.
//

#ifndef ASYNCSCHEDULER_ASYNCFUNCTION_H
#define ASYNCSCHEDULER_ASYNCFUNCTION_H

#include <functional>
#include <memory>
#include "IScheduler.h"
#include <iostream>
#include <cassert>

template<typename CallableT, typename... ARGS>
class AsyncFunction
{
public:
    explicit AsyncFunction(std::shared_ptr<IScheduler> scheduler, CallableT f)
            : function_(f), scheduler_(scheduler){ }

    void operator()(ARGS ... args)
    {
        auto lm = [this, args...]() {function_(args...);};
        if(!scheduler_->schedule(lm))
        {
            std::cerr << " operator failed" << std::endl;
        }
    }

private:
    CallableT function_;
    std::shared_ptr<IScheduler> scheduler_;
};

#endif //ASYNCSCHEDULER_ASYNCFUNCTION_H
