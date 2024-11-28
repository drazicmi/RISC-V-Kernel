#include "../h/syscall_c.h"
#include "../h/Sem.hpp"
#include "../h/Thr.hpp"


int Sem::wait() {
    if (--this->value < 0) {
        Thr* old = Thr::running;
        blocked.addLast(Thr::running);
        Thr::running = Scheduler::get();

        Thr::contextSwitch(&old->context, &Thr::running->context);
    }
    return 0;
}

int Sem::signal() {
    if ((int)++this->value<=0) {
        Thr* t = this->blocked.removeFirst();
        if (t) {
            Scheduler::put(t);
        }
    }
    return 0;
}

Sem* Sem::createSem(sem_t* handle, unsigned int init) {
    return new Sem(handle, init);
}

void Sem::closeSem(sem_t handle) {
    delete handle;
}

int Sem::tryWait() {
    if ((this->value-1) < 0) {
        return 0;
    }
    return 1;
}
