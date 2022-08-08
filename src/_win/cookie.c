
u64 __security_cookie;

void __security_init_cookie() {
    // They say it's a random number so I generated
    // one using numbergenerator.org
    __security_cookie = 0xb26e04cc62ba48a;
}

void __security_check_cookie(u64 retrieved) {
    if(__security_cookie != retrieved) {
        printf("Bro you've got a buffer overrun\n");
        abort();
    }
}
