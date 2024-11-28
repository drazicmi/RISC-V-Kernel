#include "../h/syscall_cpp.hpp"
#include "../h/Thr.hpp"

Thread::Thread (void (*body)(void*), void* arg) {
    Thr::createCoroutineNoStart(&myHandle, body, arg, new uint64[DEFAULT_STACK_SIZE]);
    this->body = body;
    this->arg = arg;
}

Thread::Thread() {
    Thr::createCoroutineNoStart(&myHandle, threadWrapper, this, new uint64[DEFAULT_STACK_SIZE]);
}

Thread::~Thread () {
    delete this->myHandle;
}

int Thread::start () {
    this->myHandle->setShouldStart(true);
    Scheduler::put(this->myHandle);
    return 0;
}

void Thread::join(){
    thread_join(this->myHandle);
}

void Thread::dispatch () {
    thread_dispatch();
}

void Thread::threadWrapper(void* arg) {
    ((Thread*)arg)->run();
};

int Thread::sleep(time_t t) {
    // Filler function for the interface of tests, it's not implemented
    return 0;
}