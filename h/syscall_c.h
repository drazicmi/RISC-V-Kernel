#ifndef PROJEKAT_FEB_SYSCALL_C_HPP
#define PROJEKAT_FEB_SYSCALL_C_HPP

#include "../lib/hw.h"

// Implementacija ove klase se oslanja na 1. zadatak sa kolokvijuma u avgustu 2021 i njegovo resenje (MemoryAlloc deo)
// MemoryAlloc functions
void* mem_alloc(size_t size);
int mem_free(void*);


// Thread functions
class Thr;
typedef Thr* thread_t;

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg);

int thread_exit();

void thread_dispatch();

void thread_join(thread_t handle);

typedef unsigned long time_t;
int time_sleep(time_t);


// Sem functions
class Sem;
typedef Sem* sem_t;

int sem_open(sem_t* handle, unsigned init);

int sem_close(sem_t handle);

int sem_wait(sem_t id);

int sem_signal (sem_t id);

int sem_trywait (sem_t id);

// Console functions
const int EOF = -1;
char getc();

void putc(char);


#endif //PROJEKAT_FEB_SYSCALL_C_HPP