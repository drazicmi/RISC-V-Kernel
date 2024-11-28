#ifndef BASE_PROJECT_SYSCALL_CPP_HPP
#define BASE_PROJECT_SYSCALL_CPP_HPP

#include "syscall_c.h"

// Interface of this class is defined by the task

void* operator new(size_t size);
void *operator new[](size_t n);
void operator delete(void* pointer);
void operator delete[](void* pointer);

class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();

    int start ();

    void join();

    static void dispatch ();
    static int sleep (time_t);

protected:
    Thread ();
    virtual void run () {}

private:
    thread_t myHandle;
    void (*body)(void*);
    void* arg;
    // Static method that handles run() overriding
    static void threadWrapper(void*);
};


class Semaphore {
public:

    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();

    int wait ();
    int signal ();
    int tryWait();

private:
    sem_t myHandle;
};


class Console {
public:
    static char getc();
    static void putc(char);
};
#endif //BASE_PROJECT_SYSCALL_CPP_HPP