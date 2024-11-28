#include "../h/MemoryAlloc.hpp"


MemoryAlloc::MemoryAlloc() {}

MemoryAlloc &MemoryAlloc::getInstanceOfMemoryAllocator() {
    // Ensure that the instance is created only once
    if (!instanceCreated) {
        // Initialize the instance
        instance.first = (FreeBlocks*)HEAP_START_ADDR;
        instance.first->size = (size_t)(((char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR - sizeof(FreeBlocks)));

        // Mark the instance as created
        instanceCreated = true;

    }
    return instance;
}

// Function allocates (at the least) size bytes of memory rounded up and aligned on block of size MEM_BLOCK_SIZE SIZE
// It returns the pointer that points to allocated part of the memory in a successful call, and NULL in a failed call.
// MEM_BLOCK_SIZE is an integer constant >= 64 or <= 1024
void *MemoryAlloc::__mem_alloc(size_t size) {
    size *= MEM_BLOCK_SIZE;
    // We grab the Singleton object, then we allocate the space based on the pointer to the free block of memory
    MemoryAlloc& instance = getInstanceOfMemoryAllocator();
    // Exception, memory can't be allocated because it's full
    if(instance.first == nullptr) {
        return nullptr;
    }
    FreeBlocks* prev = nullptr;
    FreeBlocks* blk = instance.first;

    for( ; blk!=nullptr || ( (char*)blk + size > (char*)HEAP_END_ADDR ); prev=blk, blk = blk->next)
        if(blk->size >= size) break;

    if( blk == nullptr ) {
        // Exception, memory can't be allocated
        return nullptr;
    }

    // if we are allocating in the block that has more space than required

    if (blk->size - size > 0) {
        FreeBlocks* newBlock = (FreeBlocks*)((char*)blk + size);
        if(prev) prev->next = newBlock;
        else instance.first = newBlock;
        newBlock->size = blk->size - size;
        newBlock->next = blk->next;
    }
    else {
        // if we found the block that fits perfectly we can allocate the space
        if(prev) prev->next = blk->next;
        else instance.first = blk->next;
    }
    blk->next = nullptr;
    blk->size = size;
    return (void*)((char*)blk + sizeof(FreeBlocks));

}

// Resenje se oslanja na 2. zadatak sa drugog kolokvijuma iz septembra 2015 sa adaptacijom
// da je u pitanju jednostruka umesto dvostruko ulancane lista
// druga izmena je to sto nemamo parametar size u funckiji vec moramo da ga izvucemo iz strukture FreeBlocks koja se nalazi
// neposredno pre ptr ((char*)ptr - sizeof(FreeBlocks*))
int MemoryAlloc::__mem_free(void *ptr) {
    // Find the place where to insert the new fragment (just after curr)
    MemoryAlloc &instance = getInstanceOfMemoryAllocator();
    FreeBlocks volatile *cur = 0;
    if (!instance.first || (char*)ptr < (char*)instance.first)
        // This signals that what we are actually inserting is the new beginning of the list
        cur = nullptr;
    else
        for (cur = instance.first; (cur->next != nullptr) && ( (char*)ptr > (char*)(cur->next) ); cur=cur->next);
    // Find the size of the block we are deallocating
    uint64 size = ((FreeBlocks*)((char*)ptr - sizeof(FreeBlocks)))->size;

    // Try to append it to the previous free segment (just after cur)
    if (cur && ((char*)cur + cur->size) == ((char*)ptr - sizeof(FreeBlocks))) {
        // Expand for the size of the block we are deallocating
        cur->size += size;
        // Try to join cur with next free segment
        if (cur->next && (char*)cur+cur->size == (char*)(cur->next)) {
            // Remove the cur->next segment:
            cur->size += cur->next->size;
            cur->next = cur->next->next;
        }
        return 0;
    }
    // Try to append it to the next free segment
    FreeBlocks* nxtSeg = cur ? cur->next : instance.first;
    // dodat sizeof
    if(nxtSeg && ((char*)ptr + size - sizeof(FreeBlocks)) == (char*)nxtSeg) {
        FreeBlocks* newSeg = (FreeBlocks*)((char*)ptr - sizeof(FreeBlocks));
        newSeg->size = nxtSeg->size + size;
        newSeg->next = nxtSeg->next;
        if (nxtSeg == instance.first)
            instance.first = newSeg;
        else
            cur->next = newSeg;
        return 0;
    } else {
        // No need to join; insert the new segment after cur
        FreeBlocks* newSeg = (FreeBlocks*)((char*)ptr - sizeof(FreeBlocks));
        newSeg->size = size;
        if(cur) {
            newSeg->next = cur->next;
            cur->next = newSeg;
        }
        else  {
            newSeg->next = instance.first;
            instance.first = newSeg;
        }
        return 0;
    }
}