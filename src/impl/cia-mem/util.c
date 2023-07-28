
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