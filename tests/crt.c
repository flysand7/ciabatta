
#include "testing.h"

int main(int argc, char **argv) {
    JUNIT_START("test/junit.xml");
    FEATURE_START("My feature");
    TEST(1 == 1, "Tests work");
    TESTS_PREPARE();
    TESTS_PRINT_RESULT();
    JUNIT_END();
    return 0;
}
