
#pragma once

// Common errors
#define _RT_STATUS_OK                     0  // No errors
#define _RT_ERROR_NOT_IMPLEMENTED        -1  // Function not implemented
#define _RT_ERROR_BAD_PARAM              -2  // One of the function parameters was wrong
#define _RT_ERROR_GENERIC                -3  // Just any random error

// File API errors
#define _RT_STATUS_FILE_ACCESS            1  // No access to the file
#define _RT_STATUS_FILE_NO_SPACE          2  // Storage device has no space for the file
#define _RT_STATUS_FILE_EXISTS            3  // File exists when shouldn't
#define _RT_STATUS_FILE_NOT_EXISTS        4  // File doesn't exist when should
#define _RT_STATUS_FILE_DIRECTORY         5  // The file was a directory when shouldn't've been
#define _RT_STATUS_FILE_NOT_DIRECTORY     6  // The file wasn't a directory when should've been
#define _RT_STATUS_FILE_NAME_TOO_LONG     7  // The filename was too long for the Filesystem
#define _RT_STATUS_FILE_LOOP              8  // Too many symlinks followed or a symlink encountered when expected none
#define _RT_STATUS_FILE_BUSY              9  // The device is busy if exclusive access is requested
#define _RT_STATUS_FILE_TOO_MANY_OPEN     10 // Too many open files in the process
#define _RT_STATUS_FILE_NO_MEMORY         11 // No kernel memory or user limit on memory allocation exceeded
#define _RT_STATUS_FILE_IO_ERROR          12 // I/O error
#define _RT_STATUS_FILE_BAD_FILE          13 // Bad file handle
#define _RT_STATUS_FILE_EOF               14 // Read operation reached the end-of-file

// File API flags
#define _RT_FILE_READ      0x01
#define _RT_FILE_WRITE     0x02
#define _RT_FILE_CREATE    0x04
#define _RT_FILE_EXCLUSIVE 0x08
#define _RT_FILE_TRUNCATE  0x10

typedef i32 _RT_Status;

// API implementation flags (managed and used by the layers on top of tinyrt)
static bool _rt_api_file;
static bool _rt_api_tmpfile;

// Initialization & termination of minirt
static _RT_Status _rt_init();
static _RT_Status _rt_deinit();

// Program API
#if _RT_API_PROGRAM == 1
    [[noreturn]] static void _rt_program_exit(int code);
#endif

// Environment API
#if _RT_API_ENVIRONMENT == 1
    static _RT_Status _rt_shell_exec(char const *cmd);
    static _RT_Status _rt_env_get(char const *name);
#endif

// File API
struct _RT_File typedef _RT_File;
struct _RT_File {
    union {
        void *handle;
        u64 fd;
    };
    i32 flags;
};
#if _RT_API_FILE == 1
    static _RT_File _rt_file_stdin;
    static _RT_File _rt_file_stdout;
    static _RT_File _rt_file_stderr;
    static _RT_Status _rt_file_std_handles_init();
    static _RT_Status _rt_file_open(_RT_File *file, char const *name, int flags);
    static _RT_Status _rt_file_read(u64 size, void *buffer, _RT_File *from, u64 *out_bytes_read);
    static _RT_Status _rt_file_write(_RT_File *to, u64 size, void *buffer, u64 *out_bytes_written);
    static _RT_Status _rt_file_close(_RT_File *file);
#endif

// Memory API
#if _RT_API_MEMORY == 1
    static _RT_Status _rt_mem_alloc(void *optional_desired_addr, u64 min_size, void **out_addr);
    static _RT_Status _rt_mem_free(void *ptr);
#endif
