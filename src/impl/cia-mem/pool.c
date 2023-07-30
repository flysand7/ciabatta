
static void *_pool_buffer_freelist_add(Cia_Pool *pool, u8 *buffer) {
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
    Cia_Pool_Bucket *next_bucket = pool->freelist_head;
    do {
        bucket_offset -= pool->bucket_size;
        Cia_Pool_Bucket *bucket = (Cia_Pool_Bucket *)(buffer + bucket_offset);
        bucket->next = next_bucket;
        next_bucket = bucket;
    } while(bucket_offset > header_size_aligned);
    // Add the head of the newly-created freelist to the existing freelist
    pool->freelist_head = next_bucket;
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
    pool->freelist_head = NULL;
    pool->first = allocator_alloc(&pool->allocator, pool->buffer_size, pool->alignment);
    _pool_buffer_freelist_add(pool, (u8 *)pool->first);
}

void *cia_pool_alloc(Cia_Pool *pool) {
    // If we don't have enough free buckets, create a new buffer
    if(pool->freelist_head == NULL) {
        void *buffer = allocator_alloc(&pool->allocator, pool->buffer_size, pool->alignment);
        _pool_buffer_freelist_add(pool, buffer);
    }
    // Remove item from free list and return it
    void *addr = pool->freelist_head;
    pool->freelist_head = pool->freelist_head->next;
    return addr;
}

void cia_pool_free(Cia_Pool *pool, void *ptr) {
    // Add bucket to free list
    Cia_Pool_Bucket *bucket = ptr;
    bucket->next = pool->freelist_head;
    pool->freelist_head = bucket;
}

void cia_pool_free_all(Cia_Pool *pool) {
    // Deallocate all buffers except the first one
    for(Cia_Pool_Buffer_Header *buffer = pool->first->next; buffer != NULL; buffer = buffer->next) {
        allocator_free_size(&pool->allocator, buffer, pool->buffer_size);
    }
    // Reinit the first buffer
    pool->freelist_head = NULL;
    _pool_buffer_freelist_add(pool, (u8 *)pool->first);
}

void cia_pool_destroy(Cia_Pool *pool) {
    // Simply deallocate all the buffers
    for(Cia_Pool_Buffer_Header *buffer = pool->first; buffer != NULL; buffer = buffer->next) {
        allocator_free_size(&pool->allocator, buffer, pool->buffer_size);
    }
}