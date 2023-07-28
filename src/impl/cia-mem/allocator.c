
Cia_Allocator cia_allocator_null() {
    Cia_Allocator allocator = {
        .ctx = NULL,
        .alloc = NULL,
        .alloc_a = NULL,
        .free = NULL,
        .free_all = NULL,
        .realloc = NULL
    };
    return allocator;
}

static void *page_alloc(void *ctx, u64 size) {
    void *addr;
    _RT_Status status = _rt_mem_alloc(NULL, size, &addr);
    if(status != _RT_STATUS_OK) {
        return NULL;
    }
    return addr;
}

static void page_free(void *ptr) {
    _rt_mem_free(ptr);
}

Cia_Allocator cia_allocator_pages() {
    Cia_Allocator allocator = {
        .ctx = NULL,
        .alloc = page_alloc,
        .alloc_a = NULL,
        .free = page_free,
        .free_all = NULL,
        .realloc = NULL,
    };
    return allocator;
}
