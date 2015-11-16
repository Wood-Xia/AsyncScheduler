#include <iostream>
#include <string>
#include <functional>
#include "FunctorWrapper.hpp"

using namespace std;

void greeting()
{
    cout << "Hello, World!" << endl;
}

typedef std::function<void(string, int)> CallBack;

void handle(CallBack callBack)
{
    callBack("callback", 1024);
}

void consume()
{

}

void produce()
{

}
int main()
{
    std::function<void(string, int)> foo =  [](string name, int value){cout << name << " = "
                                                                      << value << endl;};
    auto f1 = FunctorWrapper<string, int>(foo);
    handle(f1);
    auto f2 = MakeFunctorWrapper(greeting);
    f2();
    return 0;
}