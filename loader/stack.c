
#define VM_READ     0x00000001
#define VM_WRITE    0x00000002
#define VM_EXEC     0x00000004
#define VM_SHARED   0x00000008

struct File_Buffer typedef File_Buffer;
struct File_Buffer {
    int fd;
    char *data;
    u64 data_size;
    u64 last_read_size;
    u64 offset;
    int last_char;
};

struct Stack_Info typedef Stack_Info;
struct Stack_Info {
    bool stack_found;
    u64 start_addr;
    u64 end_addr;
    u32 flags;
};

static Stack_Info stack_info;

static int read_char(File_Buffer *buffer) {
    // If we need to read more stuff, fetch it from the file
    if(buffer->offset >= buffer->last_read_size) {
        int result = sys_read(buffer->fd, buffer->data, buffer->data_size);
        if(result < 0) {
            _dbg_printf("result < 0");
            return -1;
        }
        if(result == 0) {
            buffer->last_char = -1;
            return buffer->last_char;
        }
        int read_size = result;
        buffer->data[read_size] = 0;
        buffer->last_read_size = read_size;
        buffer->offset = 0;
    }
    // Return the character from a buffer
    u64 cur_offset = buffer->offset;
    buffer->offset += 1;
    buffer->last_char = buffer->data[cur_offset];
    _dbg_printf("%c", buffer->last_char);
    return buffer->last_char;
}

static int char_is_hex(int ch) {
    if('0' <= ch && ch <= '9') return 1;
    if('a' <= ch && ch <= 'f') return 1;
    if('A' <= ch && ch <= 'F') return 1;
    return 0;
}

static int char_to_hex(int ch) {
    if(ch <= '9') {
        return ch - '0';
    }
    return ch - 'a' + 10;
}

static int read_hex(File_Buffer *buffer, u64 *out_result) {
    u64 result = 0;
    int n_digits = 0;
    for(;;) {
        int ch = buffer->last_char;
        if(!char_is_hex(ch)) {
            break;
        }
        result *= 16;
        result += char_to_hex(ch);
        read_char(buffer);
        n_digits += 1;
    }
    *out_result = result;
    if(n_digits == 0) {
        return -1;
    }
    return 0;
}

static int read_dec(File_Buffer *buffer, u64 *out_result) {
    u64 result = 0;
    int n_digits = 0;
    for(;;) {
        int ch = buffer->last_char;
        if(!('0' <= ch && ch <= '9')) {
            break;
        }
        result *= 10;
        result += ch - '0';
        read_char(buffer);
        n_digits += 1;
    }
    *out_result = result;
    if(n_digits == 0) {
        return -1;
    }
    return 0;
}

static int linux_read_stack_info() {
    bool stack_found = false;
    // Initialize the buffer for buffered read
    char line_buf[1024];
    File_Buffer buffer;
    buffer.data = line_buf;
    buffer.data_size = sizeof line_buf;
    buffer.last_read_size = 0;
    buffer.offset = 0;
    // Open the file
    buffer.fd = (i32)(u32)sys_open("/proc/self/maps", 0, O_RDONLY);
    _dbg_printf("/proc/self/maps fd: %d\n", (i64)(i32)buffer.fd);
    if((i64)(i32)buffer.fd < 0) {
        return -1;
    }
    // Read the first character
    if(read_char(&buffer) < 0) {
        _dbg_printf("\nERROR: (/proc/self/maps) Failed reading char at offset %x\n", buffer.offset);
        return -1;
    }
    // Start reading the lines until we hit the stack
    char desired_name[] = "[stack]";
    while(!stack_found && buffer.last_char != -1) {
        // If we're doing n-th iteration, consume the newline
        if(buffer.last_char == '\n') {
            read_char(&buffer);
        }
        // Read the start and end addr of the section
        u64 start_addr = 0;
        u64 end_addr = 0;
        if(read_hex(&buffer, &start_addr)) {
            _dbg_printf("|\nERROR: read_hex failed to read start_addr\n");
            return -1;
        }
        if(buffer.last_char != '-') {
            _dbg_printf("|\nERROR: expected '-', didn't see it\n");
            return -1;
        }
        if(read_char(&buffer) < 0) {
            _dbg_printf("|\nERROR: read_char failed after saw '-'\n");
            return -1;
        }
        if(read_hex(&buffer, &end_addr) < 0) {
            _dbg_printf("|\nERROR: read_hex failed to read end_addr\n");
            return -1;
        }
        // Skip whitespace
        if(read_char(&buffer) < 0) {
            _dbg_printf("|\nERROR: read_char failed skipping whitespace after addrs\n");
            return -1;
        }
        // Read the flags
        u32 flags = 0;
        if(buffer.last_char == 'r') {
            flags |= VM_READ;
        }
        read_char(&buffer);
        if(buffer.last_char == 'w') {
            flags |= VM_WRITE;
        }
        read_char(&buffer);
        if(buffer.last_char == 'x') {
            flags |= VM_EXEC;
        }
        read_char(&buffer);
        if(buffer.last_char == 's') {
            flags |= VM_SHARED;
        }
        read_char(&buffer);
        // Skip 15 characters corresponding to `offset` and `device` fields
        for(int i = 0; i != 16; ++i) {
            read_char(&buffer);
        }
        // Read `inode` field
        u64 inode = 0;
        if(read_dec(&buffer, &inode)) {
            _dbg_printf("|\nERROR reading inode field: %c\n", buffer.last_char);
            return -1;
        }
        // If the line terminates here, skip this line
        if(buffer.last_char == '\n' || buffer.last_char == -1) {
            continue;
        }
        // Skip whitespace
        while(buffer.last_char == ' ') {
            read_char(&buffer);
        }
        // Read the name of the mapping
        char name_buf[256];
        u64 name_buf_size = sizeof name_buf;
        int name_len = 0;
        while(name_len < name_buf_size && buffer.last_char != '\n' && buffer.last_char != -1)
        {
            name_buf[name_len] = buffer.last_char;
            read_char(&buffer);
            ++name_len;
        }
        name_buf[name_len] = 0;
        // If the name is '[stack]' we pass the check and save the info
        if(name_len != 7) {
            continue;
        }
        for(int i = 0; i < 7; ++i) {
            if(name_buf[i] != desired_name[i]) {
                continue;
            }
        }
        // We found the stack!
        stack_info.stack_found = true;
        stack_info.start_addr = start_addr;
        stack_info.end_addr = end_addr;
        stack_info.flags = flags;
        sys_close(buffer.fd);
        _dbg_printf("\nFound stack: %x-%x\n", start_addr, end_addr);
        return 0;
    }
    sys_close(buffer.fd);
    return 1;
}
