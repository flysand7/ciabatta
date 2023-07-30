
void *cia_ptr_alignf(void *ptr, u64 alignment) {
    u64 addr = (u64)ptr;
    u64 aligned = (addr + alignment - 1) & ~(alignment - 1);
    return (void *)aligned;
}

void *cia_ptr_alignb(void *ptr, u64 alignment) {
    u64 addr = (u64)ptr;
    u64 aligned = addr & ~(alignment - 1);
    return (void *)aligned;
}

u64 cia_size_alignf(u64 size, u64 alignment) {
    return (size + alignment - 1) & ~(alignment - 1);
}

u64 cia_size_alignb(u64 size, u64 alignment) {
    return size & ~(alignment - 1);
}