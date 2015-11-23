## Asychronous Task Scheduler in C++

This is utility to wrap function object to be executed asynchronously.

### Principle

- Match TDDCPRI project specified requirement firstly
- Keep single target, not try to provide 
- Easy use
- Require smallest change on client side
- Easy to read

**TODO**

- Support schedule sequenced task or schedule task after specified task
- Support more explicit schedule policy
- Add more UT for Scheduler
- Add usage example
- Support move semantic for some API if necessory