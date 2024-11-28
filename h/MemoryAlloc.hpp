#ifndef PROJEKAT_FEB_MEMORYALLOCATOR_HPP
#define PROJEKAT_FEB_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

/* Constants that are included form the hw.h and are used to allocate memory
extern const void* HEAP_START_ADDR;
extern const void* HEAP_END_ADDR;
static const size_t MEM_BLOCK_SIZE = 64;
 */

// This struct is used to represent List Of Free Blocks that keeps the data about free fragments of memory
typedef struct FreeBlocks {
    FreeBlocks* next;
    size_t size;
} FreeBlocks;

class MemoryAlloc {
private:


    // Implementacija ove klase se oslanja na 1. zadatak sa kolokvijuma u avgustu 2021 i njegovo resenje

    // This field represents the pointer to the first block of memory
    FreeBlocks* first;

    static MemoryAlloc instance;

    static bool instanceCreated;

    // bool initialised;

    // Private constructor so no one can call it other than our getInstanceOfMemoryAllocator method
    MemoryAlloc();


public:

    // This method is used to return the single instance of the class MemoryAlloc
    // This method is essential part of the Singleton design pattern
    static MemoryAlloc& getInstanceOfMemoryAllocator();


    // These methods are static, because the MemoryAlloc class is implemented like Singleton.
    // Methods are meant to allocate and deallocate space on the heap and keep the list of FreeBlocks updated
    static void* __mem_alloc(size_t size);
    static int __mem_free(void *ptr);

};

#endif //PROJEKAT_FEB_MEMORYALLOCATOR_HPP
