
static void *null_allocator_proc(void *ctx, int optype, void *old_ptr, u64 old_size, u64 size, u64 alignment) {
    return NULL;
}

Cia_Allocator cia_allocator_null() {
    Cia_Allocator allocator = {
        .ctx = NULL,
        .proc = null_allocator_proc,
    };
    return allocator;
}

static void *page_allocator_proc(void *ctx, int optype, void *old_ptr, u64 old_size, u64 size, u64 alignment) {
    switch(optype) {
        case CIA_MEM_OP_ALLOC: {
            void *addr;
            if(alignment > 0x1000) {
                return NULL;
            }
            _RT_Status status = _rt_mem_alloc(NULL, size, &addr);
            if(status != _RT_STATUS_OK) {
                return NULL;
            }
            return addr;
        } break;
        case CIA_MEM_OP_FREE: {
            _rt_mem_free(old_ptr);
        } break;
        case CIA_MEM_OP_FREE_ALL: {
            return NULL;
        } break;
        case CIA_MEM_OP_RESIZE: {
            return NULL;
        } break;
    }
    return NULL;
}

Cia_Allocator cia_allocator_pages() {
    Cia_Allocator allocator = {
        .ctx = NULL,
        .proc = page_allocator_proc,
    };
    return allocator;
}

void *allocator_alloc(Cia_Allocator *alloc, u64 size, u64 alignment) {
    return alloc->proc(alloc->ctx, CIA_MEM_OP_ALLOC, NULL, 0, size, alignment);    
}

void allocator_free_size(Cia_Allocator *alloc, void *region_ptr, u64 region_size) {
    alloc->proc(alloc->ctx, CIA_MEM_OP_FREE, region_ptr, region_size, 0, 1);
}

void allocator_free(Cia_Allocator *alloc, void *region_ptr) {
    alloc->proc(alloc->ctx, CIA_MEM_OP_FREE, region_ptr, 0, 0, 1);
}

void allocator_free_all(Cia_Allocator *alloc) {
    alloc->proc(alloc->ctx, CIA_MEM_OP_FREE_ALL, NULL, 0, 0, 1);
}

void *allocator_resize(Cia_Allocator *alloc, void *old_ptr, u64 old_size, u64 new_size, u64 alignment) {
    return alloc->proc(alloc->ctx, CIA_MEM_OP_RESIZE, old_ptr, old_size, new_size, alignment);
}


