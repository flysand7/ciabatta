
void cia_arena_create(Cia_Arena *arena, Cia_Allocator backing_allocator, u64 buffer_size) {
    arena->allocator = backing_allocator;
    arena->buffer_size = buffer_size;
    arena->used = 0;
    arena->buffer = arena->allocator.alloc(arena->allocator.ctx, buffer_size);
}

void *cia_arena_alloc(Cia_Arena *arena, u64 size) {
    if(arena->used + size > arena->buffer_size) {
        return NULL;
    }
    void *ptr = &arena->buffer[arena->used];
    arena->used += arena->buffer_size;
    return ptr;
}

void *cia_arena_alloc_aligned(Cia_Arena *arena, u64 size, u64 align) {
    void *buffer_end = &arena->buffer[arena->buffer_size];
    void *region_ptr = cia_ptr_alignf(&arena->buffer[arena->used], align);
    void *region_end = (void *)((u64)region_ptr + size);
    if(region_end > buffer_end) {
        return NULL;
    }
    arena->used = (u64)region_ptr - (u64)arena->buffer;
    return region_ptr;
}

void cia_arena_free_all(Cia_Arena *arena) {
    arena->used = 0;
}

void cia_arena_destroy(Cia_Arena *arena) {
    arena->allocator.free(arena->allocator.ctx, arena->buffer);
}
