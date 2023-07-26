
#pragma once

// Common errors
#define RT_STATUS_OK                     0  // No errors
#define RT_ERROR_NOT_IMPLEMENTED        -1  // Function not implemented
#define RT_ERROR_BAD_PARAM              -2  // One of the function parameters was wrong

// File API errors
#define RT_STATUS_FILE_ACCESS            1  // No access to the file
#define RT_STATUS_FILE_NO_SPACE          2  // Storage device has no space for the file
#define RT_STATUS_FILE_EXISTS            3  // File exists when shouldn't
#define RT_STATUS_FILE_NOT_EXISTS        4  // File doesn't exist when should
#define RT_STATUS_FILE_DIRECTORY         5  // The file was a directory when shouldn't've been
#define RT_STATUS_FILE_NOT_DIRECTORY     6  // The file wasn't a directory when should've been
#define RT_STATUS_FILE_NAME_TOO_LONG     7  // The filename was too long for the Filesystem
#define RT_STATUS_FILE_LOOP              8  // Too many symlinks followed or a symlink encountered when expected none
#define RT_STATUS_FILE_BUSY              9  // The device is busy if exclusive access is requested
#define RT_STATUS_FILE_TOO_MANY_OPEN     10 // Too many open files in the process
#define RT_STATUS_FILE_NO_MEMORY         11 // No kernel memory or user limit on memory allocation exceeded
#define RT_STATUS_FILE_IO_ERROR          12 // I/O error
#define RT_STATUS_FILE_BAD_FILE          13 // Bad file handle
#define RT_STATUS_FILE_EOF               14 // Read operation reached the end-of-file

// File API flags
#define RT_FILE_READ      0x01
#define RT_FILE_WRITE     0x02
#define RT_FILE_CREATE    0x04
#define RT_FILE_EXCLUSIVE 0x08
#define RT_FILE_TRUNCATE  0x10

typedef i32 RT_Status;

// API implementation flags (managed and used by the layers on top of tinyrt)
static bool _rt_api_file;
static bool _rt_api_tmpfile;

// Initialization & termination of minirt
static RT_Status rt_init();
static RT_Status rt_deinit();

// Program API
#if RT_API_PROGRAM == 1
    static noreturn void rt_program_exit(int code);
#endif

#if RT_API_ENVIRONMENT == 1
    static RT_Status rt_shell_exec(char const *cmd);
    static RT_Status rt_env_get(char const *name);
#endif

// File API
#if RT_API_FILE == 1
    struct RT_File typedef RT_File;
    struct RT_File {
        union {
            void *handle;
            u64 fd;
        };
        i32 flags;
    };
    static RT_Status rt_file_open(RT_File *file, char *name, int flags);
    static RT_Status rt_file_read(u64 size, void *buffer, RT_File *from, u64 *out_bytes_read);
    static RT_Status rt_file_write(RT_File *to, u64 size, void *buffer, u64 *out_bytes_written);
    static RT_Status rt_file_close(RT_File *file);
#endif
