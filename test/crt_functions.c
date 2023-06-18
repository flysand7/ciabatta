
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

// Tested
#include <stdint.h>
#include <limits.h>
#include <ctype.h>

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

static void printd(int number) {
    if(number < 0) {
        putchar('-');
        number = -number;
    }
    char buffer[20] = {0};
    char *str = buffer + sizeof buffer;
    do {
        *--str = number%10+'0';
        number /= 10;
    } while(number != 0);
    prints(str);
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
    bool is_succeeded;
};

static void print_test_results(Test_Feature *features_head) {
    int total_test_count = 0;
    int total_success_count = 0;
    for(Test_Feature *feature = features_head; feature != NULL; feature = feature->next) {
        total_test_count += feature->test_count;
        total_success_count += feature->success_count;
        term_color_green();
        prints("==> Feature ");
        term_color_reset();
        prints(feature->name);
        prints(": (");
        printd(feature->success_count);
        printc('/');
        printd(feature->test_count);
        prints(")\n");
        if(feature->success_count < feature->test_count) {
            int test_index = 0;
            for(Test *test = feature->test_head; test != NULL; test = test->next) {
                if(!test->is_succeeded) {
                    term_color_red();
                    prints("  Test #");
                    printd(1+test_index);
                    term_color_reset();
                    // prints(" (");
                    // prints(test->condition_str);
                    prints(" failed: ");
                    prints(test->error_msg);
                    printc('\n');
                }
                test_index += 1;
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
    }
    prints("TESTS COMPLETED: ");
    printd(total_success_count);
    printc('/');
    printd(total_test_count);
    term_color_reset();
    printc('\n');
}

#define XSTR(expr) #expr
#define STR(expr) XSTR(expr)

#define TESTS_INIT() \
    Test_Feature *current_feature = NULL

#define FEATURE_START__(NAME, NUMBER) \
    Test_Feature feature_ ## NUMBER; \
    (feature_ ## NUMBER).next = current_feature; \
    current_feature = &(feature_ ## NUMBER); \
    current_feature->name = NAME; \
    current_feature->test_head = NULL; \
    current_feature->success_count = 0; \
    current_feature->test_count = 0

#define FEATURE_START_(NAME, NUMBER) \
    FEATURE_START__(NAME, NUMBER)    

#define FEATURE_START(NAME) \
    FEATURE_START_(NAME, __COUNTER__)

#define FEATURE_END()

#define TEST__(EXPR, ERROR_MSG, NUMBER) \
    Test test_ ## NUMBER; \
    (test_ ## NUMBER).next = current_feature->test_head; \
    current_feature->test_head = &test_ ## NUMBER; \
    current_feature->test_head->condition_str = STR(EXPR); \
    current_feature->test_head->error_msg = ERROR_MSG; \
    current_feature->test_head->is_succeeded = EXPR; \
    if(current_feature->test_head->is_succeeded) {\
        current_feature->success_count += 1; \
    }\
    current_feature->test_count += 1

#define TEST_(EXPR, ERROR_MSG, NUMBER) \
    TEST__(EXPR, ERROR_MSG, NUMBER)

#define TEST(EXPR, ERROR_MSG) \
    TEST_(EXPR, ERROR_MSG, __COUNTER__)

#define TESTS_PRINT_RESULT() \
    print_test_results(current_feature)

int main(int argc, char **argv) {
    TESTS_INIT();
    
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
        // Test letters
        for(int i = 0; i != 10; ++i) {
            TEST(isalpha(random_between('a', 'z')) != 0, "isalpha(letter) returned false");
            TEST(isdigit(random_between('a', 'z')) == 0, "isdigit(letter) returned true");
        }
        // Test digits
        for(int i = 0; i != 10; ++i) {
            TEST(isalpha(random_between('0', '9')) == 0, "isalpha(digit) returned true");
            TEST(isdigit(random_between('0', '9')) != 0, "isdigit(digit) returned false");
        }
    FEATURE_END();

    TESTS_PRINT_RESULT();
    return 0;
}
