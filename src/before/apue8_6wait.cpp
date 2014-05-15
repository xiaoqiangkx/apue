#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

void log_err(const char *s) {
    printf("%s\n", s);
    exit(-1);
}

int main(int argc, char* argv[]) {
    int a = 1;
    int num = 2;
    int pid[2] = {0};

    if ( (pid[0] = fork()) < 0) {
        log_err("fork failed.\n");
    } else if (pid[0] == 0) {
        sleep(4);
        return 0;
    }    

    if ( (pid[1] = fork()) < 0) {
        log_err("fork failed.\n");
    } else if (pid[1] == 0) {
        sleep(2);
        return 0;
    }    

    int count = 0;
    int *statloc = (int*)malloc(sizeof(int));
    int id = -1;
    while (count < num) {
        id = wait(statloc);
        if (id == -1) log_err("wait failed.\n");
        else {
            printf("exit pid=%d\n", id);
            printf("%d, %d, %d, %d\n", WIFEXITED(statloc), WIFSIGNALED(statloc), WIFSTOPPED(statloc), WIFCONTINUED(statloc));
        }
        count++;
    }

    if(!statloc) free(statloc);
    return 0;
}
