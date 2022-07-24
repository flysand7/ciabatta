
// TODO: the code for rand/srand was shamelessly copied
// from C11 standard's example and is a simple LCG.
// gotta implement a better random function, or no idc

static int last_random_number;

int rand(void) {
    last_random_number = last_random_number * 1103515245 + 12345;
    return (unsigned int)(last_random_number/65536) % RAND_MAX;
}

void srand(unsigned int seed) {
    last_random_number = seed;
}
