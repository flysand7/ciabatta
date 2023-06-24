
// Framework for testing of the CRT library. This file is supposed to be linked
// to the ciabatta, so there is a possibility that the runtime that the testing
// suite relies on is broken, which is why to decrease the chance of it crashing
// because of that, I minimize that dependency. Therefore this testing suite
// avoids the following:
// - Heap allocations
// - Calls to high-level functions like printf, preferring low-level fwrite instead
// - Calling other CRT functions other than for the purpose of testing them

// Dependencies
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdint.h>

// Tested
#include <stdint.h>
#include <limits.h>
#include <ctype.h>
#include <time.h>

#define TEST_MEMORY_SIZE 8*1024*1024
static uint8_t test_memory[TEST_MEMORY_SIZE];
static uint64_t test_memory_head = TEST_MEMORY_SIZE;

static void *mem_alloc(uint64_t size) {
    if(test_memory_head < size) {
        fputs("Out of memory. Can't continue testing!!", stderr);
        return NULL;
    }
    test_memory_head -= size;
    return &test_memory[test_memory_head];
}

static unsigned long random_seed = 0;

unsigned long random(void) {
   random_seed = random_seed * 2147001325 + 715136305;
   return 0x31415926 ^ ((random_seed >> 16) + (random_seed << 16));
}

unsigned long random_between(int lo, int hi) {
    return (random() % (1+hi - lo)) + lo;
}

static void printc(char c) {
    putchar(c);
}

static void prints(char *str) {
    while(*str != 0) {
        printc(*str++);
    }
}

static void printd(int32_t number, int width) {
    if(number < 0) {
        putchar('-');
        number = -number;
    }
    char buffer[20] = {0};
    char *str = buffer + sizeof buffer - 1;
    do {
        *--str = number%10+'0';
        number /= 10;
    } while(number != 0);
    int num_digits = (int)((buffer + sizeof buffer - 1) - str);
    int pad_width = width - num_digits;
    while(pad_width-- > 0) {
        printc('0');
    }
    prints(str);
}

static void printu(uint32_t number, int width) {
    char buffer[20] = {0};
    char *str = buffer + sizeof buffer;
    do {
        *--str = number%10+'0';
        number /= 10;
    } while(number != 0);
    int num_digits = (int)((buffer + sizeof buffer - 1) - str);
    int pad_width = width - num_digits;
    while(pad_width-- > 0) {
        printc('0');
    }
    prints(str);
}

static void print_fmt(char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    while(*fmt != 0) {
        if(*fmt != '%') {
            printc(*fmt);
        }
        else {
            ++fmt;
            int width = 0;
            while('0' <= *fmt && *fmt <= '9') {
                width = 10*width + *fmt-'0';
                ++fmt;
            }
            if(*fmt == 'c') {
                int ch = va_arg(args, int);
                printc(ch);
            }
            else if(*fmt == '%') {
                printc('%');
            }
            else if(*fmt == 's') {
                char *str = va_arg(args, char*);
                prints(str);
            }
            else if(*fmt == 'd') {
                int32_t i = va_arg(args, int32_t);
                printd(i, width);
            }
            else if(*fmt == 'u') {
                uint32_t u = va_arg(args, uint32_t);
                printu(u, width);
            }
        }
        ++fmt;
    }
    va_end(args);
}

static void sprint_fmt(char *dst, char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    while(*fmt != 0) {
        if(*fmt != '%') {
            *dst++ = *fmt;
        }
        else {
            ++fmt;
            if(*fmt == 'c') {
                int ch = va_arg(args, int);
                *dst++ = ch;
            }
            else if(*fmt == 's') {
                char *str = va_arg(args, char*);
                while((*dst++ = *str++));
            }
            else if(*fmt == 'd') {
                int32_t i = va_arg(args, int32_t);
                if(i < 0) {
                    i = -i;
                    *dst++ = '-';
                }
                char buffer[20] = {0};
                char *str = buffer + sizeof buffer;
                do {
                    *--str = i%10+'0';
                    i /= 10;
                } while(i != 0);
                while((*dst++ = *str++));
            }
            else if(*fmt == 'u') {
                uint32_t u = va_arg(args, uint32_t);
                char buffer[20] = {0};
                char *str = buffer + sizeof buffer;
                do {
                    *--str = u%10+'0';
                    u /= 10;
                } while(u != 0);
                while((*dst++ = *str++));                
            }
        }
        ++fmt;
    }
    *dst = 0;
    va_end(fmt);
}

static void term_color_green() {
    prints("\x1b[32m");
}

static void term_color_red() {
    prints("\x1b[31m");
}

static void term_color_yellow() {
    prints("\x1b[33m");
}

static void term_color_reset() {
    prints("\x1b[0m");
}

// This stuff is for saving results of tests to be a bit more flexible with printing
// test results
struct Test typedef Test;
struct Test_Feature typedef Test_Feature;

struct Test_Feature {
    Test_Feature *next;
    char *name;
    int test_count;
    int success_count;
    Test *test_head;
};

struct Test {
    Test *next;
    char *condition_str;
    char *error_msg;
    int line;
    bool is_succeeded;
};

static void print_test_results(Test_Feature *features_reversed) {
    prints(":: Printing test results\n");
    // Reverse the linked list of features
    Test_Feature *new_head = NULL;
    while(features_reversed != NULL) {
        Test_Feature *reversed_next = features_reversed->next;
        Test_Feature *new_prev = features_reversed;
        new_prev->next = new_head;
        new_head = new_prev;
        features_reversed = reversed_next;
    }
    // Iterate features
    int total_test_count = 0;
    int total_success_count = 0;
    for(Test_Feature *feature = new_head; feature != NULL; feature = feature->next) {
        // Update counters
        total_test_count += feature->test_count;
        total_success_count += feature->success_count;
        // Print feature name
        term_color_green();
        print_fmt("==> Feature ");
        term_color_reset();
        print_fmt("%s: (%d/%d)\n", feature->name, feature->success_count, feature->test_count);
        if(feature->success_count < feature->test_count) {
            // Reverse the linked list
            Test *reversed_head = feature->test_head;
            Test *head = NULL;
            while(reversed_head != NULL) {
                Test *reversed_next = reversed_head->next;
                Test *head_prev = reversed_head;
                head_prev->next = head;
                head = head_prev;
                reversed_head = reversed_next;
            }
            // Iterate tests
            int test_index = 0;
            for(Test *test = head; test != NULL; test = test->next) {
                if(!test->is_succeeded) {
                    term_color_red();
                    print_fmt("  Test #%d", 1+test_index);
                    term_color_reset();
                    print_fmt(" failed (line %d): %s\n", test->line, test->error_msg);
                }
                test_index += 1;
            }
        }
    }
    float success_percentage = (float) total_success_count / (float)total_test_count;
    if(success_percentage < 0.5) {
        term_color_red();
    }
    else if(success_percentage != 1.0) {
        term_color_yellow();
    }
    else {
        term_color_green();
    }
    time_t timestamp = time(NULL);
    struct tm tm = *localtime(&timestamp);
    print_fmt("[%4d-%2d-%2d %2d:%2d:%2d] ", 1900+tm.tm_year, 1+tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    prints("TESTS COMPLETED: ");
    printd(total_success_count, 0);
    printc('/');
    printd(total_test_count, 0);
    term_color_reset();
    printc('\n');
}

#define XSTR(expr) #expr
#define STR(expr) XSTR(expr)

Test_Feature *current_feature = NULL;

#define FEATURE_START__(NAME, NUMBER) \
    { \
        print_fmt(":: Running tests for %s\n", NAME); \
        Test_Feature *feature = mem_alloc(sizeof(Test_Feature)); \
        feature->next = current_feature; \
        current_feature = feature; \
        current_feature->name = NAME; \
        current_feature->test_head = NULL; \
        current_feature->success_count = 0; \
        current_feature->test_count = 0; \
    }

#define FEATURE_START_(NAME, NUMBER) \
    FEATURE_START__(NAME, NUMBER)    

#define FEATURE_START(NAME) \
    FEATURE_START_(NAME, __COUNTER__)

#define FEATURE_END()

#define TEST__(EXPR, ERROR_FMT, NUMBER, LINE, ...) \
    { \
        Test *test = mem_alloc(sizeof(Test)); \
        test->next = current_feature->test_head; \
        current_feature->test_head = test; \
        current_feature->test_head->condition_str = STR(EXPR); \
        current_feature->test_head->is_succeeded = EXPR; \
        current_feature->test_head->line = LINE; \
        if(current_feature->test_head->is_succeeded) {\
            current_feature->success_count += 1; \
        }\
        else { \
            current_feature->test_head->error_msg = mem_alloc(256); \
            sprint_fmt(current_feature->test_head->error_msg, ERROR_FMT, __VA_ARGS__); \
        }\
        current_feature->test_count += 1; \
    }

#define TEST_(EXPR, ERROR_MSG, NUMBER, LINE, ...) \
    TEST__(EXPR, ERROR_MSG, NUMBER, LINE, __VA_ARGS__)

#define TEST(EXPR, ERROR_MSG, ...) \
    TEST_(EXPR, ERROR_MSG, __COUNTER__, __LINE__, __VA_ARGS__)

#define TESTS_PRINT_RESULT() \
    print_test_results(current_feature)

int main(int argc, char **argv) {
    FEATURE_START("limits.h");
    {
        // Check existing of macro definitions
        #ifdef BOOL_WIDTH
            TEST(BOOL_WIDTH == 8*sizeof(bool), "BOOL_WIDTH isn't correlated with sizeof(bool)");
        #else
            TEST(false, "The macro BOOL_WIDTH wasn't defined");
        #endif
        #ifdef CHAR_WIDTH
            TEST(CHAR_WIDTH == 8, "CHAR_WIDTH isn't 8");
        #else
            TEST(false, "The macro CHAR_WIDTH wasn't defined");
        #endif
        #ifdef CHAR_BIT
            TEST(CHAR_BIT == 8, "CHAR_BIT isn't 8");
        #else
            TEST(false, "The macro CHAR_BIT wasn't defined");
        #endif
        #ifdef CHAR_MIN
            TEST(CHAR_MIN == -128, "CHAR_MIN isn't -128");
        #else
            TEST(false, "The macro CHAR_MIN wasn't defined");
        #endif
        #ifdef CHAR_MAX
            TEST(CHAR_MAX == 127, "CHAR_MAX isn't 127");
        #else
            TEST(false, "The macro CHAR_MAX wasn't defined");
        #endif
        #ifdef MB_LEN_MAX
            TEST(true, "");
        #else
            TEST(false, "The macro MB_LEN_MAX wasn't defined");
        #endif
        
        #ifdef SCHAR_WIDTH
            TEST(SCHAR_WIDTH == 8, "SCHAR_WIDTH isn't 8");
        #else
            TEST(false, "The macro SCHAR_WIDTH wasn't defined");
        #endif
        #ifdef SHRT_WIDTH
            TEST(SHRT_WIDTH == 16, "SHRT_WIDTH isn't 16");
        #else
            TEST(false, "The macro SHRT_WIDTH wasn't defined");
        #endif
        #ifdef INT_WIDTH
            TEST(INT_WIDTH == 32, "INT_WIDTH isn't 32");
        #else
            TEST(false, "The macro INT_WIDTH wasn't defined");
        #endif
        #ifdef LONG_WIDTH
            TEST(LONG_WIDTH == 32, "LONG_WIDTH isn't 32");
        #else
            TEST(false, "The macro LONG_WIDTH wasn't defined");
        #endif
        #ifdef LLONG_WIDTH
            TEST(LLONG_WIDTH == 64, "LLONG_WIDTH isn't 64");
        #else
            TEST(false, "The macro LLONG_WIDTH wasn't defined");
        #endif
        
        #ifdef SCHAR_MIN
            TEST(SCHAR_MIN == -128, "SCHAR_MIN isn't -128");
        #else
            TEST(false, "The macro SCHAR_MIN wasn't defined");
        #endif
        #ifdef SHRT_MIN
            TEST(SHRT_MIN == -0x8000, "SHRT_MIN isn't -0x8000");
        #else
            TEST(false, "The macro SHRT_MIN wasn't defined");
        #endif
        #ifdef INT_MIN
            TEST(INT_MIN == -0x80000000, "INT_MIN isn't -0x80000000");
        #else
            TEST(false, "The macro INT_MIN wasn't defined");
        #endif
        #ifdef LONG_MIN
            TEST(LONG_MIN == -0x80000000l, "LONG_MIN isn't -0x80000000");
        #else
            TEST(false, "The macro LONG_MIN wasn't defined");
        #endif
        #ifdef LLONG_MIN
            TEST(LLONG_MIN == -0x8000000000000000ll, "LLONG_MIN isn't -0x8000000000000000");
        #else
            TEST(false, "The macro LLONG_MIN wasn't defined");
        #endif
        
        #ifdef SCHAR_MAX
            TEST(SCHAR_MAX == 127, "SCHAR_MAX isn't 127");
        #else
            TEST(false, "The macro SCHAR_MAX wasn't defined");
        #endif
        #ifdef SHRT_MAX
            TEST(SHRT_MAX == 0x7fff, "SHRT_MAX isn't 0x7fff");
        #else
            TEST(false, "The macro SHRT_MAX wasn't defined");
        #endif
        #ifdef INT_MAX
            TEST(INT_MAX == 0x7fffffff, "INT_MAX isn't 0x7fffffff");
        #else
            TEST(false, "The macro INT_MAX wasn't defined");
        #endif
        #ifdef LONG_MAX
            TEST(LONG_MAX == 0x7fffffff, "LONG_MAX isn't 0x7fffffff");
        #else
            TEST(false, "The macro LONG_MAX wasn't defined");
        #endif
        #ifdef LLONG_MAX
            TEST(LLONG_MAX == 0x7fffffffffffffffll, "LLONG_MAX isn't 0x7fffffffffffffff");
        #else
            TEST(false, "The macro LLONG_MAX wasn't defined");
        #endif
        
        #ifdef UCHAR_WIDTH
            TEST(UCHAR_WIDTH == 8, "UCHAR_WIDTH isn't 8");
        #else
            TEST(false, "The macro UCHAR_WIDTH wasn't defined");
        #endif
        #ifdef USHRT_WIDTH
            TEST(USHRT_WIDTH == 16, "USHRT_WIDTH isn't 16");
        #else
            TEST(false, "The macro USHRT_WIDTH wasn't defined");
        #endif
        #ifdef UINT_WIDTH
            TEST(UINT_WIDTH == 32, "UINT_WIDTH isn't 32");
        #else
            TEST(false, "The macro UINT_WIDTH wasn't defined");
        #endif
        #ifdef ULONG_WIDTH
            TEST(ULONG_WIDTH == 32, "ULONG_WIDTH isn't 32");
        #else
            TEST(false, "The macro ULONG_WIDTH wasn't defined");
        #endif
        #ifdef ULLONG_WIDTH
            TEST(ULLONG_WIDTH == 64, "ULLONG_WIDTH isn't 64");
        #else
            TEST(false, "The macro ULLONG_WIDTH wasn't defined");
        #endif
        
        #ifdef UCHAR_MAX
            TEST(UCHAR_MAX == 255, "UCHAR_MAX isn't 255");
        #else
            TEST(false, "The macro UCHAR_MAX wasn't defined");
        #endif
        #ifdef USHRT_MAX
            TEST(USHRT_MAX == 0xffffu, "USHRT_MAX isn't 0xffff");
        #else
            TEST(false, "The macro USHRT_MAX wasn't defined");
        #endif
        #ifdef UINT_MAX
            TEST(UINT_MAX == 0xffffffffu, "UINT_MAX isn't 0xffffffff");
        #else
            TEST(false, "The macro UINT_MAX wasn't defined");
        #endif
        #ifdef ULONG_MAX
            TEST(ULONG_MAX == 0xffffffffu, "ULONG_MAX isn't 0xffffffff");
        #else
            TEST(false, "The macro ULONG_MAX wasn't defined");
        #endif
        #ifdef ULLONG_MAX
            TEST(ULLONG_MAX == 0xffffffffffffffffull, "ULLONG_MAX isn't 0xffffffffffffffffull");
        #else
            TEST(false, "The macro ULLONG_MAX wasn't defined");
        #endif
            
        #ifdef PTRDIFF_WIDTH
            TEST(true, "");
        #else
            TEST(false, "The macro PTRDIFF_WIDTH isn't defined");
        #endif
        #ifdef PTRDIFF_MIN
            TEST(true, "");
        #else
            TEST(false, "The macro PTRDIFF_MIN isn't defined");
        #endif
        #ifdef PTRDIFF_MAX
            TEST(true, "");
        #else
            TEST(false, "The macro PTRDIFF_MAX isn't defined");
        #endif
        #ifdef SIZE_WIDTH
            TEST(true, "");
        #else
            TEST(false, "The macro SIZE_WIDTH isn't defined");
        #endif
        #ifdef SIZE_MAX
            TEST(true, "");
        #else
            TEST(false, "The macro SIZE_MAX isn't defined");
        #endif
        #ifdef SIG_ATOMIC_WIDTH
            TEST(true, "");
        #else
            TEST(false, "The macro SIG_ATOMIC_WIDTH isn't defined");
        #endif
            #ifdef SIG_ATOMIC_MIN
            TEST(true, "");
        #else
            TEST(false, "The macro SIG_ATOMIC_MIN isn't defined");
        #endif
            #ifdef SIG_ATOMIC_MAX
            TEST(true, "");
        #else
            TEST(false, "The macro SIG_ATOMIC_MAX isn't defined");
        #endif
        #ifdef WINT_WIDTH
            TEST(true, "");
        #else
            TEST(false, "The macro WINT_WIDTH isn't defined");
        #endif
            #ifdef WINT_MIN
            TEST(true, "");
        #else
            TEST(false, "The macro WINT_MIN isn't defined");
        #endif
            #ifdef WINT_MAX
            TEST(true, "");
        #else
            TEST(false, "The macro WINT_MAX isn't defined");
        #endif
        #ifdef WCHAR_WIDTH
            TEST(true, "");
        #else
            TEST(false, "The macro WCHAR_WIDTH isn't defined");
        #endif
        #ifdef WCHAR_MIN
            TEST(true, "");
        #else
            TEST(false, "WCHAR_MIN isn't defined");
        #endif
            #ifdef WCHAR_MAX
            TEST(true, "");
        #else
            TEST(false, "WCHAR_MAX isn't defined");
        #endif
    }
    FEATURE_END();

    FEATURE_START("ctype.h");
    {
        // Test letters
        for(int i = 0; i != 10; ++i) {
            int letter = random_between('a', 'z');
            TEST(isalpha(letter) != 0, "isalpha('%c') returned false", letter);
            TEST(isdigit(letter) == 0, "isdigit('%c') returned true", letter);
        }
        // Test digits
        for(int i = 0; i != 10; ++i) {
            int digit = random_between('0', '9');
            TEST(isalpha(digit) == 0, "isalpha('%c') returned true", digit);
            TEST(isdigit(digit) != 0, "isdigit('%c') returned false", digit);
        }
    }
    FEATURE_END();
    
    FEATURE_START("IO functions (stdio.h)");
    {
        static int numbers[10] = {0,1,2,3,4,5,6,7,8,9};
        // fread/fwrite
        {
            FILE *file = fopen("test_folder/file", "wb");
            TEST(file != NULL, "Created file is NULL");
            int cnt_written = fwrite(numbers, sizeof(int), 10, file);
            TEST(cnt_written == 10, "fwrite didn't write all 10 objects");
            TEST(fclose(file) == 0, "fclose didn't close the file");
            TEST(rename("test_folder/file", "test_folder/file2") == 0, "Rename returned 0");
            file = fopen("test_folder/file2", "rb");
            TEST(file != NULL, "Created file is NULL");
            int read_numbers[10];
            int cnt_read = fread(numbers, sizeof(int), 10, file);
            TEST(cnt_read == 10, "fread didn't read 10 objects");
            bool all_ok = true;
            for(int i = 0; i != 10; ++i) {
                if(read_numbers[i] != numbers[i]) {
                    all_ok = false;
                    break;
                }
            }
            TEST(all_ok, "The elements read didn't match the elements written");
            TEST(fclose(file) == 0, "fclose didn't close the file");
            TEST(remove("test_folder/file2") == 0, "remove didn't remove the file");
        }
        // Seeking and stuff
        {
            FILE *file = fopen("test_folder/seek", "wb");
            TEST(file != NULL, "Created file is NULL");
            TEST(fwrite(numbers, sizeof(int), 10, file) == 10, "fwrite didn't write all 10 objects");
            TEST(fflush(file) == 0, "fflush failed");
            TEST(fseek(file, 4*sizeof(int), SEEK_SET) == 0, "fseek couldn't seek to offset 4");
            int num;
            TEST(fread(&num, sizeof(int), 1, file) == 1, "fread didn't read the int");
            TEST(num == 4, "Wrong value read at offset 4");
            TEST(fseek(file, -4, SEEK_END) == 0, "fseek coudn't seek to the end");
            TEST(fread(&num, sizeof(int), 1, file) == 1, "fread didn't read the int");
            TEST(num == 9, "Wrong number read");
            TEST(fclose(file) == 0, "fclose didn't close the file");
            file = fopen("test_folder/seek", "wb");
            TEST(file != NULL, "Created file is NULL");
            fpos_t nul_pos;
            TEST(fgetpos(file, &nul_pos) == 0, "Couldn't get file position");
            TEST(fseek(file, 0, SEEK_END) == 0, "Couldn't seek to the end of the file");
            long file_size = ftell(file);
            TEST(file_size != -1L, "ftell failed");
            TEST(fsetpos(file, &nul_pos) == 0, "Couldn't reset file position");
            TEST(ftell(file) == 0, "File position reset to wrong spot");
        }
        // Getchar, ungetchar
        {
            char str[] = "Hello, world!";
            FILE *file = fopen("test_folder/getc", "wb");
            TEST(file != NULL, "Created file is NULL");
            TEST(fputs(str, file) >= 0, "fputs failed");
            TEST(fputc('!', file) == '!', "fputc failed");
            TEST(fflush(file) == 0, "fflush failed");
            // TEST(fclose(file) == 0, "fclose failed");
            file = freopen("test_folder/getc", "rb", file);
            TEST(file != NULL, "Reopened file is NULL");
            TEST(fgets(str, sizeof str, file) == str, "fgets failed");
            TEST(fgetc(file) == '!', "fgetc read the wrong thing");
            TEST(ungetc('?', file) == '?', "ungetc failed to put a character");
            TEST(fgetc(file) == '?', "Didn't get back the same character that we unget'ed");
            TEST(fclose(file) == 0, "fclose failed");
        }
    }

    TESTS_PRINT_RESULT();
    return 0;
}
