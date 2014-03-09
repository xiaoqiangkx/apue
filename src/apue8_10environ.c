#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int i;
    char **ptr;
    extern char **environ;

    for (i = 0; i < argc; i++) {
        printf("argc[%d]: %s\n", i, argv[i]);
    } 

    for (ptr = environ; *ptr != (char*)0; ptr++) {
        printf("env: %s\n", *ptr);
    }
    return 0;
}
