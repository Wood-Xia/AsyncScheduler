## Asychronous Task Scheduler in C++

This is utility to wrap function object to be executed asynchronously.

### Principle

- Match project specified requirement firstly
- Keep single target, not try to provide full functionality
- Easy use
- Require smallest change on client side
- Easy to read

### Example

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


**TODO**

- Support schedule sequenced task or schedule task after specified task
- Support more explicit schedule policy
- Add more UT for Scheduler
- Add usage example
- Support move semantic for some API if necessory