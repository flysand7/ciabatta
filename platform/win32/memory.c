
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#include "win32.h"

// TODO: lock the heap before allocation (?)

HANDLE _heap;

static bool is_power_of_two(size_t s) {
    return (s & (s-1)) == 0;
}

static intptr_t align_forward(intptr_t p, size_t a) {
    return (p+a-1)&~(a-1);
}

void _heap_setup(_os_heap *heap) {
    _heap = heap->handle;
}

void *aligned_alloc(size_t alignment, size_t size) {
    if(size == 0) {
        return NULL;
    }
    if(alignment == 0) {
        alignment = 8;
    }
    if(!is_power_of_two(alignment)) {
        return NULL;
    }
    // HeapAlloc is guaranteed to return 8-byte aligned pointer,
    // so if our alignment is 8 or less we don't have to overallocate.
    // otherwise we'll reserve extra `alignment` bytes to later adjust our
    // memory buffer according to required alignment.
    size_t min_req_size = size;
    if(alignment > 8) {
        min_req_size += alignment;
    }
    void *block_start = HeapAlloc(_heap, HEAP_ZERO_MEMORY, size+alignment);
    intptr_t block_start_i = (intptr_t)block_start;
    intptr_t aligned_block_start_i = align_forward(block_start_i, alignment);
    void *aligned_block_start = (void *)aligned_block_start_i;
    return aligned_block_start;
}

void *calloc(size_t nmemb, size_t size) {
    if(nmemb == 0 || size == 0) {
        return NULL;
    }
    if(nmemb > SIZE_MAX/size) {
        return NULL;
    }
    void *block_start = HeapAlloc(_heap, HEAP_ZERO_MEMORY, size*nmemb);
    return block_start;
}

void free(void *ptr) {
    if(ptr == NULL) {
        return;
    }
    HeapFree(_heap, 0, ptr);
}

void *malloc(size_t size) {
    return aligned_alloc(8, size);
}

void *realloc(void *ptr, size_t size) {
    void *buffer = HeapReAlloc(_heap, 0, ptr, size);
    return buffer;
}
