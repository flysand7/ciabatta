
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
#include <time.h>

// Tested
#include <stdint.h>
#include <limits.h>
#include <ctype.h>

// MEMORY

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

// RANDOM NUMBER GENERATOR (RNG)

static unsigned long random_seed = 0;

unsigned long random(void) {
   random_seed = random_seed * 2147001325 + 715136305;
   return 0x31415926 ^ ((random_seed >> 16) + (random_seed << 16));
}

unsigned long random_between(int lo, int hi) {
    return (random() % (1+hi - lo)) + lo;
}

// FORMATTING AND IO

bool fmt_xml_escapes = false;

static void fprintc(FILE *file, char c) {
    fputc(c, file);
}

static void fprints(FILE *file, char *str) {
    while(*str != 0) {
        fputc(*str++, file);
    }
}

static void fprintc_maybe_xml(FILE *file, char c) {
    if(c == '"' && fmt_xml_escapes) {
        fprints(file, "&quot;");
    }
    else if(c == '&' && fmt_xml_escapes) {
        fprints(file, "&amp;");
    }
    else if(c == '<' && fmt_xml_escapes) {
        fprints(file, "&lt;");
    }
    else if(c == '>' && fmt_xml_escapes) {
        fprints(file, "&gt;");
    }
    else if(c == '\'' && fmt_xml_escapes) {
        fprints(file, "&apos;");
    }
    else {
        fprintc(file, c);
    }
}

static void fprintd(FILE *file, int32_t number, int width) {
    if(number < 0) {
        fprintc(file, '-');
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
        fprintc(file, '0');
    }
    fprints(file, str);
}

static void fprintu(FILE *file, uint32_t number, int width) {
    char buffer[20] = {0};
    char *str = buffer + sizeof buffer;
    do {
        *--str = number%10+'0';
        number /= 10;
    } while(number != 0);
    int num_digits = (int)((buffer + sizeof buffer - 1) - str);
    int pad_width = width - num_digits;
    while(pad_width-- > 0) {
        fprintc(file, '0');
    }
    fprints(file, str);
}

static void fvprint_fmt(FILE *file, char *fmt, va_list args) {
    while(*fmt != 0) {
        if(*fmt != '%') {
            fprintc(file, *fmt);
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
                fprintc_maybe_xml(file, ch);
            }
            else if(*fmt == '%') {
                fprintc(file, '%');
            }
            else if(*fmt == 's') {
                char *str = va_arg(args, char*);
                while(*str != 0) {
                    fprintc_maybe_xml(file, *str);
                    ++str;
                }
            }
            else if(*fmt == 'd') {
                int32_t i = va_arg(args, int32_t);
                fprintd(file, i, width);
            }
            else if(*fmt == 'u') {
                uint32_t u = va_arg(args, uint32_t);
                fprintu(file, u, width);
            }
        }
        ++fmt;
    }
}

static void fprint_fmt(FILE *file, char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fvprint_fmt(file, fmt, args);
    va_end(args);
}

static void printc(char c) {
    fprintc(stdout, c);
}

static void prints(char *str) {
    fprints(stdout, str);
}

static void printd(int32_t number, int width) {
    fprintd(stdout, number, width);
}

static void printu(uint32_t number, int width) {
    fprintu(stdout, number, width);
}

static void print_fmt(char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fvprint_fmt(stdout, fmt, args);
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
            int width = 0;
            while('0' <= *fmt && *fmt <= '9') {
                width = 10*width + *fmt-'0';
                ++fmt;
            }
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
                int num_digits = (int)((buffer + sizeof buffer - 1) - str);
                int pad_width = width - num_digits;
                while(pad_width-- > 0) {
                    *dst++ = '0';
                }
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
                int num_digits = (int)((buffer + sizeof buffer - 1) - str);
                int pad_width = width - num_digits;
                while(pad_width-- > 0) {
                    *dst++ = '0';
                }
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

// TEST SUITE FUNCTIONS

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

static Test_Feature *reverse_test_lists(Test_Feature *features_reversed) {
    Test_Feature *new_head = NULL;
    while(features_reversed != NULL) {
        Test_Feature *reversed_next = features_reversed->next;
        Test_Feature *new_prev = features_reversed;
        new_prev->next = new_head;
        new_head = new_prev;
        features_reversed = reversed_next;
    }
    for(Test_Feature *feature = new_head; feature != NULL; feature = feature->next) {
        Test *reversed_head = feature->test_head;
        Test *head = NULL;
        while(reversed_head != NULL) {
            Test *reversed_next = reversed_head->next;
            Test *head_prev = reversed_head;
            head_prev->next = head;
            head = head_prev;
            reversed_head = reversed_next;
        }
        feature->test_head = head;
    }
    return new_head;
}

static void print_test_results(Test_Feature *features) {
    prints(":: Printing test results\n");
    // Iterate features
    int total_test_count = 0;
    int total_success_count = 0;
    for(Test_Feature *feature = features; feature != NULL; feature = feature->next) {
        // Update counters
        total_test_count += feature->test_count;
        total_success_count += feature->success_count;
        // Print feature name
        term_color_green();
        print_fmt("==> Feature ");
        term_color_reset();
        print_fmt("%s: (%d/%d)\n", feature->name, feature->success_count, feature->test_count);
        if(feature->success_count < feature->test_count) {
            int test_index = 0;
            for(Test *test = feature->test_head; test != NULL; test = test->next) {
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

// JUNIT OUTPUT

static void junit_write(char *path, Test_Feature *features) {
    fmt_xml_escapes = true;
    FILE *xml = fopen(path, "wb");
    // TODO: store tests and failures in an object instead of calculating it like that
    int total_test_count = 0;
    int total_success_count = 0;
    for(Test_Feature *feature = features; feature != NULL; feature = feature->next) {
        total_test_count += feature->test_count;
        total_success_count += feature->success_count;
    }
    fprint_fmt(xml, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
    fprint_fmt(xml, "<testsuites id=\"0\" name=\"%s\" tests=\"%d\" failures=\"%d\" time=\"0\">\n",
        "Ciabatta CRT functions test suite", total_test_count, total_test_count - total_success_count);
    int feature_id = 0;
    for(Test_Feature *feature = features; feature != NULL; feature = feature->next) {
        fprint_fmt(xml, "  <testsuite id=\"%d\" name=\"%s\" tests=\"%d\" failures=\"%d\" time=\"0\">\n",
            feature_id, feature->name, feature->test_count, feature->test_count - feature->success_count);
        int test_id = 0;
        for(Test *test = feature->test_head; test != NULL; test = test->next) {
            fprint_fmt(xml, "    <testcase id=\"%d\" name=\"%s\" time=\"0\">\n",
                test_id, test->condition_str);
            if(!test->is_succeeded) {
                fprint_fmt(xml, "      <failure message=\"crt.c(%d): %s\" type=\"ERROR\">\n",
                    test->line, test->error_msg);
                fprint_fmt(xml, "crt.c(%d):\n  %s\n", test->line, test->error_msg);
                fprint_fmt(xml, "      </failure>\n");
            }
            test_id += 1;
            fprint_fmt(xml, "    </testcase>\n");
        }
        feature_id += 1;
        fprint_fmt(xml, "  </testsuite>\n");
    }
    fprint_fmt(xml, "</testsuites>\n");
    fclose(xml);
    fmt_xml_escapes = false;
}

// TEST MACROS

#define XSTR(expr) #expr
#define STR(expr) XSTR(expr)

Test_Feature *current_feature = NULL;
bool junit_output = false;
char *junit_output_path = NULL;

#define JUNIT_START(XML_PATH) \
    junit_output = true; \
    junit_output_path = XML_PATH

#define JUNIT_END() \
    if(junit_output) { \
        junit_write(junit_output_path, current_feature); \
    }

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

#define TESTS_PREPARE() \
    current_feature = reverse_test_lists(current_feature)

#define TESTS_PRINT_RESULT() \
    print_test_results(current_feature)

