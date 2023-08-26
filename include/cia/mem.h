
#pragma once

#if !defined(KB)
    #define KB ((i64)1024)
#endif

#if !defined(MB)
    #define MB ((i64)1024*KB)
#endif

#if !defined(GB)
    #define GB ((i64)1024*MB)
#endif

#if !defined(TB)
    #define TB ((i64)1024*GB)
#endif

void *cia_ptr_alignf(void *ptr, u64 alignment);
void *cia_ptr_alignb(void *ptr, u64 alignment);
u64 cia_size_alignf(u64 size, u64 alignment);
u64 cia_size_alignb(u64 size, u64 alignment);

#define CIA_MEM_OP_ALLOC     1
#define CIA_MEM_OP_FREE      2
#define CIA_MEM_OP_FREE_ALL  3
#define CIA_MEM_OP_RESIZE    4

struct Cia_Allocator typedef Cia_Allocator;
struct Cia_Allocator {
    void *ctx;
    void *(*proc)(void *ctx, int optype, void *old_ptr, u64 old_size, u64 size, u64 alignment);
};

Cia_Allocator cia_allocator_null();
Cia_Allocator cia_allocator_pages();

void *cia_allocator_alloc(Cia_Allocator *alloc, u64 size, u64 alignment);
void cia_allocator_free_size(Cia_Allocator *alloc, void *region_ptr, u64 region_size);
void cia_allocator_free(Cia_Allocator *alloc, void *region_ptr);
void cia_allocator_free_all(Cia_Allocator *alloc);
void *cia_allocator_resize(Cia_Allocator *alloc, void *old_ptr, u64 old_size, u64 new_size, u64 alignment);

struct Cia_Arena typedef Cia_Arena;
struct Cia_Arena {
    Cia_Allocator allocator;
    u64 buffer_size;
    u64 used;
    u8* buffer;
};

void cia_arena_create(Cia_Arena *arena, Cia_Allocator backing_allocator, u64 max_size);
void *cia_arena_alloc(Cia_Arena *arena, u64 size);
void *cia_arena_alloc_aligned(Cia_Arena *arena, u64 size, u64 align);
void cia_arena_free_all(Cia_Arena *arena);
void cia_arena_destroy(Cia_Arena *arena);

struct Cia_Pool_Bucket typedef Cia_Pool_Bucket;
struct Cia_Pool_Bucket {
    Cia_Pool_Bucket *next;
};

struct Cia_Pool_Buffer_Header typedef Cia_Pool_Buffer_Header;
struct Cia_Pool_Buffer_Header {
    Cia_Pool_Buffer_Header *next;
};

struct Cia_Pool typedef Cia_Pool;
struct Cia_Pool {
    Cia_Allocator allocator;
    Cia_Pool_Buffer_Header *first;
    Cia_Pool_Bucket *freelist_head;
    u64 buffer_size;
    u64 bucket_size;
    u64 alignment;
};

void cia_pool_create(Cia_Pool *pool, Cia_Allocator backing_allocator, u64 buffer_size, u64 element_size, u64 alignment);
void *cia_pool_alloc(Cia_Pool *pool);
void cia_pool_free(Cia_Pool *pool, void *ptr);
void cia_pool_free_all(Cia_Pool *pool);
void cia_pool_destroy(Cia_Pool *pool);

