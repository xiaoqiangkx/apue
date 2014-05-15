#include <unistd.h>
#include <stdio.h>

int main() {
    int a = 1;

    FILE* file = fopen("test.txt", "w");
    fwrite("123", 3, 1, file);

    if (fork() == 0) {
        a++;
        fwrite("abc", 3, 1, file);
        printf("id=%d, a=%d\n", getpid(), a);
    } else {
        sleep(2);
        fwrite("edf", 3, 1, file);
        printf("id=%d, a=%d\n", getpid(), a);
    } 

    if (file) fclose(file);
    return 0;
}
