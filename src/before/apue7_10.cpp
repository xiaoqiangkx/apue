#include <setjmp.h>
#include <stdio.h>

jmp_buf env;

void h2() { longjmp(env, 1); }
void h1() { h2(); }

int main() {
    volatile int val = 1;

    if (setjmp(env) != 0) {
        printf("++\n");
    }

    val = 2;
    h1();
    return 0;
}
