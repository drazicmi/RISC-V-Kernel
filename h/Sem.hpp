#ifndef BASE_PROJECT_SEM_HPP
#define BASE_PROJECT_SEM_HPP

#include "scheduler.hpp"
#include "syscall_c.h"

class Sem {
private:

    friend class Thr;
    int value;
    List<Thr> blocked;

public:

    Sem(sem_t* handle, unsigned init) {
        this->value = (int)init;
    }

    ~Sem() {
        // Empty the list
        while(Thr* b = blocked.removeFirst()){
            Scheduler::put(b);
        }
        this->value = 0;
    }

    int wait();
    int signal();
    int tryWait();

    static Sem* createSem(sem_t* handle, unsigned init);
    static void closeSem(sem_t handle);

};

#endif //BASE_PROJECT_SEM_HPP