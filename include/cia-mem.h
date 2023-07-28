
#pragma once

void *cia_ptr_alignf(void *ptr, u64 alignment);
void *cia_ptr_alignb(void *ptr, u64 alignment);

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

void *allocator_alloc(Cia_Allocator *alloc, u64 size, u64 alignment);
void allocator_free_size(Cia_Allocator *alloc, void *region_ptr, u64 region_size);
void allocator_free(Cia_Allocator *alloc, void *region_ptr);
void allocator_free_all(Cia_Allocator *alloc);
void *allocator_resize(Cia_Allocator *alloc, void *old_ptr, u64 old_size, u64 new_size, u64 alignment);

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
