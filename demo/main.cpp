#include <iostream>
#include <string>
#include <utility>
#include <functional>
#include <memory>
#include <chrono>
#include <thread>
#include <vector>
#include "AsyncFunction.h"
#include "IScheduler.h"
#include "Scheduler.h"

using namespace std;

typedef std::function<void(string, int)> CallableT;

void produce(SchedulerPtr scheduler)
{
    int value = 0;

    // original callable object
    CallableT foo =  [](string name, int value)
    {
        cout << name << " = " << value << endl;
    };

    // wrapper callable object by AsyncFunction
    AsyncFunction<CallableT, string, int> aFunction(scheduler, foo);
    bool noEndless = false;
    while (value++ < 1000000 || noEndless)
    {
        // now callback is executed asynchronously
        aFunction(string("name"), value);
    }
}

int main()
{
    try
    {
        auto p = std::make_shared<Scheduler>();
        p->start();

        std::vector<std::thread> producers;
        for (size_t i = 0; i < 10; i++)
        {
            producers.push_back(std::thread(produce, p));
        }

        while(p->notCompletedTask() > 0)
        {
            std::this_thread::yield();
        }
        for (auto& it: producers)
        {
            it.join();
        }
        assert(p->notCompletedTask() == 0);
        p->stop();
    }
    catch (...)
    {
        std::cout << "exception catched" << std::endl;
    }


    return 0;
}