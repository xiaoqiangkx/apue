#include <unistd.h>
#include <stdio.h>

int main() {
    int a = 1;

    if (fork() == 0) {
        a++;
        printf("id=%d, a=%d\n", getpid(), a);
    } else {
        sleep(2);
        printf("id=%d, a=%d\n", getpid(), a);
    } 
    return 0;
}
