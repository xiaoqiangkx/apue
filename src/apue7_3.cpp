#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

static void func1() {
    printf("hello1\n");
}

static void func2() {
    printf("hello2\n");
}

int main(int argc, char* argv[]) {
    assert(argc == 2);
    int a = atoi(argv[1]);

    int rc = atexit(func1);
    assert(rc == 0);

    rc = atexit(func2);
    assert(rc == 0);

    switch(a) {
        case 0:
           exit(0);
           break; 
        case 1:
           _Exit(0);
           break;
    }
    return 0;
}
