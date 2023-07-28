
#pragma once

void *cia_ptr_alignf(void *ptr, u64 alignment);
void *cia_ptr_alignb(void *ptr, u64 alignment);

struct Cia_Allocator typedef Cia_Allocator;
struct Cia_Allocator {
    void *ctx;
    void *(*alloc)(void *ctx, u64 size);
    void *(*alloc_a)(void *ctx, u64 size, u64 alignment);
    void (*free)(void *ctx, void *ptr);
    void (*free_all)(void *ctx, void *ptr);
    void *(*realloc)(void *ctx, void *ptr, u64 new_size);
};

Cia_Allocator cia_allocator_null();
Cia_Allocator cia_allocator_pages();

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
