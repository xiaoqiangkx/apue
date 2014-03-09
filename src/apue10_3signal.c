#include <signal.h>
#include <stdio.h>

void handler(int);

int main() {
    if (signal(SIGUSR1, handler) == SIG_ERR) {
        printf("Error.\n");
        return -1;
    }

    if (signal(SIGUSR2, handler) == SIG_ERR) {
        printf("Error.\n");
        return -1;
    }

    while (1) {}
    return 0;
}

void handler(int signo) {
    if (signo == SIGUSR1) {
        printf("SIGUSR1\n");
    } else if (signo == SIGUSR2) {
        printf("SIGUSR2\n");
    } else {
        printf("BIG ERROR\n");
    }
}
