#include "../h/MemoryAlloc.hpp"
#include "../h/riscv.hpp"
#include "../h/Thr.hpp"

// Initialize static members
MemoryAlloc MemoryAlloc::instance;
bool MemoryAlloc::instanceCreated = false;


extern void userMain();

void init_user_mode() {
    // Main thread is in system mode, in order to prevent access to forbidden instructions we have to switch
    // to user_mode before running the tests
    uint64 callArgument = 0x100UL;

    __asm__ volatile ("mv a0, %0" : : "r" (callArgument));
    __asm__ volatile ("ecall");
}

void nitRadnik(void*) {
    while(true) {
        thread_dispatch();
    }
}

void wrapperUserMain(void*) {
    userMain();
}

int main()
{
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    init_user_mode();
//    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

//    void* ptr1 = mem_alloc(40);
//    void* ptr2 = mem_alloc(50);
//    void* ptr3 = mem_alloc(64);
//    mem_free(ptr1);
//    ptr1 = mem_alloc(50);
//    void* ptr4 = mem_alloc(40);
//    void* ptr5 = mem_alloc(40);
//    printInteger((uint64)ptr1);
//    printString("\n");
//    printInteger((uint64)ptr2);
//    printString("\n");
//    printInteger((uint64)ptr3);
//    printString("\n");
//    printInteger((uint64)ptr4);
//    printString("\n");
//    printInteger((uint64)ptr5);
//    printString("\n");
//    mem_free(ptr3);
//    mem_free(ptr1);

//

//    printString("Coroutine worker created\n");
//
//    thread_create(&coroutines[1], threadJoinRadilica, nullptr);
//    printString("Coroutine threadJoinRadilica created\n");
//
//    thread_create(&coroutines[2], threadJoinCekalica, (void*)coroutines[1]);
//    printString("Coroutine threadJoinCekalica created\n");
//
//
//    while (!(coroutines[1]->isFinished()) && !(coroutines[2]->isFinished())) {
//        Thr::yield();
//    }
//
//    for (auto &coroutine: coroutines)
//    {
//        delete coroutine;
//    }
//    printString("Finished Main\n");

    // Create empty thread so scheduler won't be empty
    Thr *coroutine;
    thread_create(&coroutine, nullptr, nullptr);
    Thr::running = coroutine;


    Thr* idle;
    thread_create(&idle, nitRadnik, nullptr);

    Thr* handleMain;
    thread_create(&handleMain, wrapperUserMain, nullptr);
    thread_join(handleMain);

    delete coroutine;
    // Possibly delete other threads
    return 0;
}
