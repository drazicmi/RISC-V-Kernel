#include "../h/syscall_c.h"
#include "../h/MemoryAlloc.hpp"

// Function allocates (at the least) size bytes of memory rounded up and aligned on block of size MEM_BLOCK_SIZE SIZE
// It returns the pointer that points to allocated part of the memory in a successful call, and NULL in a failed call.
// MEM_BLOCK_SIZE is an integer constant >= than 64 or <= 1024
// Function is used as a wrapper that calls the interrupt using ecall
void* mem_alloc(size_t size) {

    // EXCEPTION (MEM_BLOCK_SIZE) out of bounds
    if (MEM_BLOCK_SIZE < 64 || MEM_BLOCK_SIZE > 1024) return nullptr;
    uint64 sizeArgument = (uint64)( (size + (size_t)sizeof(FreeBlocks))  / MEM_BLOCK_SIZE);
    if (( size + (size_t)sizeof(FreeBlocks) % MEM_BLOCK_SIZE) > 0) sizeArgument++;
    // pass the arguments to the interrupt routine by writing them in a0 and a1
    uint64 callArgument = 0x01UL;

    __asm__ volatile ("mv a0, %0" : : "r" (callArgument));
    __asm__ volatile ("mv a1, %0" : : "r" (sizeArgument));
    __asm__ volatile ("ecall");

    // get the returning pointer and return it
    uint64 retPointer;
    __asm__ volatile ("mv %0, a0" : "=r" (retPointer));
    return (void*)retPointer;
}

int mem_free(void *ptr) {
    // Argument has to have value that is returned by mem_alloc. If that is not the case, the behaviour is undefined...
    // EXCEPTION (pointer is null)
    if (ptr == nullptr || ptr < HEAP_START_ADDR || ptr > HEAP_END_ADDR) return -1;
    // pass the arguments to the interrupt routine by writing them in a0 and a1
    uint64 callArgument = 0x02UL;

    // we have to save the ptr (its in reg a0) before writing in a0
    __asm__ volatile ("mv a1, %0" : : "r" (ptr));
    __asm__ volatile ("mv a0, %0" : : "r" (callArgument));
    __asm__ volatile ("ecall");

    // get the returning pointer and return it
    uint64 retValue;
    __asm__ volatile ("mv %0, a0" : "=r" (retValue));
    return int(retValue);
}

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg) {
    // Create the thread that starts running upon a creation

    uint64 callArgument = 0x11UL;
    void* stack_space = mem_alloc(DEFAULT_STACK_SIZE);
    __asm__ volatile ("mv a4, %0" : : "r"(stack_space));
    __asm__ volatile ("mv a3, %0" : : "r"(arg));
    __asm__ volatile ("mv a2, %0" : : "r"(start_routine));
    __asm__ volatile ("mv a1, %0" : : "r" (handle));
    __asm__ volatile ("mv a0, %0" : : "r" (callArgument));
    __asm__ volatile ("ecall");

    // get the returning value and return it
    uint64 retValue;
    __asm__ volatile ("mv %0, a0" : "=r" (retValue));
    return int(retValue);
}

int thread_exit() {
    // Shut down the running thread
    uint64 callArgument = 0x12UL;

    __asm__ volatile ("mv a0, %0" : : "r" (callArgument));
    __asm__ volatile ("ecall");

    // get the returning value and return it
    uint64 retValue;
    __asm__ volatile ("mv %0, a0" : "=r" (retValue));
    if (retValue == 0) {
        thread_dispatch();
    }
    return int(retValue);
}

void thread_dispatch() {
    // Change the running thread (possibly)
    uint64 callArgument = 0x13UL;

    __asm__ volatile ("mv a0, %0" : : "r" (callArgument));
    __asm__ volatile ("ecall");

    // no returning value
}

void thread_join(thread_t handle) {
    // The running thread waits full execution of thread that handle points to
    uint64 callArgument = 0x14UL;

    __asm__ volatile ("mv a1, %0" : : "r" (handle));
    __asm__ volatile ("mv a0, %0" : : "r" (callArgument));
    __asm__ volatile ("ecall");

    // No returning value
}

int time_sleep(time_t time) {
    // Dummy implementation, always returns 0

    uint64 callArgument = 0x31UL;

    __asm__ volatile ("mv a1, %0" : : "r" (time));
    __asm__ volatile ("mv a0, %0" : : "r" (callArgument));
    __asm__ volatile ("ecall");

    // get the returning value and return it
    uint64 retValue;
    __asm__ volatile ("mv %0, a0" : "=r" (retValue));
    return int(retValue);
}

// Sem functions implementation
int sem_open(sem_t* handle, unsigned init) {

    uint64 callArgument = 0x21UL;

    __asm__ volatile ("mv a2, %0" : : "r"(init));
    __asm__ volatile ("mv a1, %0" : : "r"(handle));
    __asm__ volatile ("mv a0, %0" : : "r" (callArgument));
    __asm__ volatile ("ecall");

    // get the returning value and return it
    uint64 retValue;
    __asm__ volatile ("mv %0, a0" : "=r" (retValue));
    return int(retValue);
}

int sem_close(sem_t handle) {

    uint64 callArgument = 0x22UL;

    __asm__ volatile ("mv a1, %0" : : "r"(handle));
    __asm__ volatile ("mv a0, %0" : : "r" (callArgument));
    __asm__ volatile ("ecall");

    // get the returning value and return it
    uint64 retValue;
    __asm__ volatile ("mv %0, a0" : "=r" (retValue));
    return int(retValue);
}

int sem_wait(sem_t handle) {

    uint64 callArgument = 0x23UL;

    __asm__ volatile ("mv a1, %0" : : "r"(handle));
    __asm__ volatile ("mv a0, %0" : : "r" (callArgument));
    __asm__ volatile ("ecall");

    // get the returning value and return it
    uint64 retValue;
    __asm__ volatile ("mv %0, a0" : "=r" (retValue));
    return (int)retValue;
}

int sem_signal(sem_t handle) {

    uint64 callArgument = 0x24UL;

    __asm__ volatile ("mv a1, %0" : : "r"(handle));
    __asm__ volatile ("mv a0, %0" : : "r" (callArgument));
    __asm__ volatile ("ecall");

    // get the returning value and return it
    uint64 retValue;
    __asm__ volatile ("mv %0, a0" : "=r" (retValue));
    return (int)retValue;
}

int sem_trywait (sem_t handle) {
    uint64 callArgument = 0x26UL;

    __asm__ volatile ("mv a1, %0" : : "r"(handle));
    __asm__ volatile ("mv a0, %0" : : "r" (callArgument));
    __asm__ volatile ("ecall");

    // get the returning value and return it
    uint64 retValue;
    __asm__ volatile ("mv %0, a0" : "=r" (retValue));
    return (int)retValue;
}

// Console functions

char getc() {

    uint64 callArgument = 0x41UL;

    __asm__ volatile ("mv a0, %0" : : "r" (callArgument));
    __asm__ volatile ("ecall");

    // get the returning value and return it
    uint64 retValue;
    __asm__ volatile ("mv %0, a0" : "=r" (retValue));
    return (char)retValue;
}

void putc(char c) {

    uint64 callArgument = 0x42UL;

    __asm__ volatile ("mv a1, %0" : : "r"(c));
    __asm__ volatile ("mv a0, %0" : : "r" (callArgument));
    __asm__ volatile ("ecall");
}