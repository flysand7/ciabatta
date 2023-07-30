
static void pool_reinit_buffer(Cia_Pool *pool, u8 *buffer) {
    Cia_Pool_Buffer_Header *header = (Cia_Pool_Buffer_Header *)buffer;
    u64 header_size = sizeof(Cia_Pool_Buffer_Header);
    u64 header_size_aligned = cia_size_alignf(header_size, pool->alignment);
    u8 *buckets = buffer + header_size_aligned;
    u64 remaining_size = pool->buffer_size - header_size_aligned;
    u64 buckets_count = remaining_size / pool->bucket_size;
    header->free_buckets_count = buckets_count;
    u64 buckets_size = buckets_count * pool->bucket_size;
    // Initialize every bucket as free
    u64 bucket_offset = header_size_aligned + buckets_size;
    Cia_Pool_Bucket *next_bucket = NULL;
    do {
        bucket_offset -= pool->bucket_size;
        Cia_Pool_Bucket *bucket = (Cia_Pool_Bucket *)(buffer + bucket_offset);
        bucket->next = next_bucket;
        next_bucket = bucket;
    } while(bucket_offset > header_size_aligned);
    header->free_head = next_bucket;
}

void cia_pool_create(Cia_Pool *pool, Cia_Allocator backing_allocator, u64 buffer_size, u64 bucket_size, u64 alignment) {
    pool->allocator = backing_allocator;
    pool->buffer_size = buffer_size;
    pool->bucket_size = bucket_size;
    pool->alignment = alignment;
    // If the unallocated element can't hold a bucket, increase the size of the element
    if(pool->bucket_size < sizeof(Cia_Pool_Bucket)) {
        pool->bucket_size = sizeof(Cia_Pool_Bucket);
    }
    // Make element size a multiple of the alignment
    pool->bucket_size = cia_size_alignf(pool->bucket_size, pool->alignment);
    // Allocate and initialize the first buffer
    pool->first = allocator_alloc(&pool->allocator, pool->buffer_size, pool->alignment);
    pool_reinit_buffer(pool, (u8 *)pool->first);
}

void *cia_pool_alloc(Cia_Pool *pool) {
    Cia_Pool_Buffer_Header *buffer = pool->first;
    // Find buffer with free data in it
    while(buffer != NULL) {
        if(buffer->free_buckets_count > 0) {
            break;
        }
        buffer = buffer->next;
    }
    // If none have it, then create a new buffer
    if(buffer == NULL) {
        void *buffer_mem = allocator_alloc(&pool->allocator, pool->buffer_size, pool->alignment);
        pool_reinit_buffer(pool, buffer_mem);
        Cia_Pool_Buffer_Header *header = buffer_mem;
        header->next = pool->first;
        pool->first = header;
        buffer = header;
    }
    // Remove item from free list and return it
    void *addr = buffer->free_head;
    buffer->free_head = buffer->free_head->next;
    return addr;
}

void cia_pool_free(Cia_Pool *pool, void *ptr) {
    // Check which buffer the allocation is from
    Cia_Pool_Buffer_Header *source = NULL;
    for(Cia_Pool_Buffer_Header *buffer = pool->first; buffer != NULL; buffer = buffer->next) {
        void *buffer_start = buffer;
        void *buffer_end = (u8 *)buffer + pool->buffer_size;
        if(buffer_start < ptr && ptr < buffer_end) {
            source = buffer;
            break;
        }
    }
    // Add bucket to free list
    Cia_Pool_Bucket *bucket = ptr;
    bucket->next = source->free_head;
    source->free_head = bucket;
}

void cia_pool_free_all(Cia_Pool *pool) {
    // Deallocate all buffers except the first one
    for(Cia_Pool_Buffer_Header *buffer = pool->first->next; buffer != NULL; buffer = buffer->next) {
        allocator_free_size(&pool->allocator, buffer, pool->buffer_size);
    }
    // Reinit the first buffer
    pool_reinit_buffer(pool, (u8 *)pool->first);
}

void cia_pool_destroy(Cia_Pool *pool) {
    // Simply deallocate all the buffers
    for(Cia_Pool_Buffer_Header *buffer = pool->first; buffer != NULL; buffer = buffer->next) {
        allocator_free_size(&pool->allocator, buffer, pool->buffer_size);
    }
}