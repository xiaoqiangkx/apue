#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void change(char * restrict p) {
    char *q = p;
    q[0] = 'a';
}

int main(int argc, char *argv[]) {

    char *p = malloc(sizeof(char) * 10);
    memcpy(p, "helloworl", 10);
    change(p);
    printf("%s\n", p);
    return 0;
}
