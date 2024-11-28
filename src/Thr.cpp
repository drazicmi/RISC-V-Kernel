#include "../h/Thr.hpp"

Thr *Thr::running = nullptr;

int Thr::createCoroutineStart(thread_t *handle, Body body, void *arg, void *stack_space)
{
    Thr* t = new Thr(handle, body, arg, stack_space, true);
    if (t) return 0;
    return -1;
}

int Thr::createCoroutineNoStart(thread_t *handle, Thr::Body body, void *arg, void *stack_space) {
    Thr* t = new Thr(handle, body, arg, stack_space, false);
    if (t) return 0;
    return -1;
}


void Thr::yield()
{
    Riscv::pushRegisters();

    Thr::dispatch();

    Riscv::popRegisters();
}

void Thr::dispatch()
{
    Thr *old = running;
    if (!old->isFinished()) { Scheduler::put(old); }
    running = Scheduler::get();

    Thr::contextSwitch(&old->context, &running->context);
}


void Thr::join(thread_t handle) {
    // Function is supposed to busy-wait until the thread that handle points to is not finished
    while(!(handle->isFinished()))
        yield();
}

int Thr::thread_exit() {
    this->setFinished(true);

    // Free the stack
    if (this->stack != nullptr) {
        delete[] this->stack;
        this->stack = nullptr;
    }

    //  Set body to nullptr
    if (this->body != nullptr) {
          this->body = nullptr;
    }

    return 0;
}

void Thr::threadWrapper() {
    // return to user mode
    Riscv::popSppSpie();
    while(!running->shouldStart)
        thread_dispatch();
    // Invoke the running thread with given argument
    running->body(running->arg);
    running->setFinished(true);
    thread_dispatch();
}

bool Thr::isShouldStart() const {
    return shouldStart;
}

void Thr::setShouldStart(bool shouldStart) {
    Thr::shouldStart = shouldStart;
}





