
#define DEFAULT_ALIGNMENT 16

static HANDLE heap_handle;

static bool _is_power_of_two(size_t s) {
    return (s & (s-1)) == 0;
}

static intptr_t _align_forward(intptr_t p, size_t a) {
    return (p+a-1)&~(a-1);
}


static void _setup_heap() {
    heap_handle = GetProcessHeap();
    if (heap_handle == NULL) {
        ExitProcess(-42069);
    }
}

struct AllocationTag typedef AllocationTag;
struct AllocationTag {
    void *ptr;
    size_t size;
};

// Retrieve the tag using allocated pointer
static AllocationTag *_mem_block_tag(void *user_ptr) {
    AllocationTag *tag = (void *)((char *)user_ptr - sizeof(AllocationTag));
    return tag;
}

// Calculate the block size that is enough to hold user data, the tag, and
// the necessary free space in case the pointer from HeapAlloc needs further
// alignment
static size_t _mem_min_block_size(size_t alignment, size_t user_size) {
    size_t size = user_size + sizeof(AllocationTag);
    if(alignment > 8) {
        size += alignment;
    }
    return size;
}

// Calculate the start of the user data, such that it's aligned, attach the tag
// to the allocated pointer
static void *_mem_block_setup(void *block_start, size_t block_size, size_t alignment) {
    intptr_t block_start_i = (intptr_t)block_start;
    intptr_t aligned_block_start_i = _align_forward(block_start_i, alignment);
    intptr_t free_space = aligned_block_start_i - block_start_i;
    if(free_space < sizeof(AllocationTag)) {
        aligned_block_start_i += alignment;
    }
    AllocationTag *tag = _mem_block_tag(block_start);
    tag->ptr = block_start;
    tag->size = block_size;
    return (void *)aligned_block_start_i;
}

void *_mem_alloc(size_t alignment, size_t size) {
    // HeapAlloc is guaranteed to return 8-byte aligned pointer,
    // so if our alignment is 8 or less we don't have to overallocate.
    // otherwise we'll reserve extra `alignment` bytes to later adjust our
    // memory buffer according to required alignment.
    size_t block_size = _mem_min_block_size(alignment, size);
    void *block_start = HeapAlloc(heap_handle, 0, block_size);
    if(block_start == NULL) {
        return NULL;
    }
    return _mem_block_setup(block_start, block_size, alignment);
}

void *aligned_alloc(size_t alignment, size_t size) {
    if(size == 0) {
        return NULL;
    }
    if(alignment == 0) {
        alignment = 8;
    }
    if(!_is_power_of_two(alignment)) {
        return NULL;
    }
    return _mem_alloc(alignment, size);
}

void *calloc(size_t nmemb, size_t size) {
    if(nmemb == 0 || size == 0) {
        return NULL;
    }
    if(nmemb > SIZE_MAX/size) {
        return NULL;
    }
    return _mem_alloc(DEFAULT_ALIGNMENT, nmemb * size);
}

void free(void *ptr) {
    if(ptr == NULL) {
        return;
    }
    AllocationTag *tag = (void *)((char *)ptr - sizeof(AllocationTag));
    HeapFree(heap_handle, 0, tag->ptr);
}

void *malloc(size_t size) {
    if(size == 0) {
        return NULL;
    }
    return _mem_alloc(DEFAULT_ALIGNMENT, size);
}

void *realloc(void *ptr, size_t size) {
    if (ptr == NULL) {
        if (size == 0) {
            return NULL;
        }
        return _mem_alloc(DEFAULT_ALIGNMENT, size);
    } else if (size == 0) {
        free(ptr);
        return NULL;
    } else {
        AllocationTag *tag = _mem_block_tag(ptr);
        void *old_block = tag->ptr;
        size_t new_block_size = _mem_min_block_size(DEFAULT_ALIGNMENT, size);
        void *new_block = HeapReAlloc(heap_handle, 0, old_block, new_block_size);
        return _mem_block_setup(new_block, new_block_size, DEFAULT_ALIGNMENT);
    }
}

