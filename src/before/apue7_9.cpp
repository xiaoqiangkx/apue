#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    char *s = getenv("PATH");
    printf("PATH is %s\n", s);
    printf("HOME is %s\n", getenv("HOME"));
    return 0;
}
