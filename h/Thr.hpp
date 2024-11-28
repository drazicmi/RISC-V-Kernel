#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_CCB_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_CCB_HPP

#include "scheduler.hpp"
#include "syscall_c.h"
#include "../h/riscv.hpp"
#include "../test/printing.hpp"

// Coroutine Control Block
class Thr
{
public:
    ~Thr() { delete[] stack; }

    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }


    using Body = void (*)(void*);

    // Static methods that serve for creation of the threads, with starting them, and without starting them
    static int createCoroutineStart(thread_t *handle, Body body, void *arg, void *stack_space);
    static int createCoroutineNoStart(thread_t *handle, Body body, void *arg, void *stack_space);

    static void yield();

    static Thr *running;

    int thread_exit();

    void join(thread_t handle);


private:
    explicit Thr(thread_t* handle, Body body, void* arg, void* stack_space, bool shouldStart)
    {
        this->body = body;
        this->stack = (uint64*) stack_space;
        this->context.ra = body != nullptr ? (uint64) &threadWrapper : 0;
        this->context.sp = stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE]: 0;
        this->arg = arg;
        this->finished = false;
        this->shouldStart = shouldStart;
        *handle = this;
        // Only put threads that are supposed to start into scheduler
        if (shouldStart) {
            Scheduler::put(this);
        }
    }

    // Context of the thread that contains stack pointer and returning address
    struct Context
    {
        // Return address of the thread
        uint64 ra;
        // Thr's stack pointer
        uint64 sp;
    };

    // For every thread we save her data (body, stack, context)
    Body body;
    // Beginning of the stack
    uint64 *stack;
    Context context;
    // Indicator that the thread finished her job
    bool finished;
    // Threads argument
    void* arg;
    // Indicator whether the thread should start or not
    bool shouldStart;

public:
    // Getters and setters
    bool isShouldStart() const;

    void setShouldStart(bool shouldStart);

private:

    friend class Sem;

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();

    static uint64 constexpr STACK_SIZE = 1024;

    static void threadWrapper();
};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_CCB_HPP