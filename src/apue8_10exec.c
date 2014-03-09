#include <unistd.h>
#include <stdio.h>

char* env[] = {"USER=unknown", "PATH=.", NULL};

int main() {
//    int rc = execl("/usr/local/nginx/sbin/nginx", "nginx",  (char*)0);
//    if (rc == -1) printf("Failed.\n");
//    else printf("Success.\n");
//    return 0;
    pid_t pid;

    if ((pid = fork()) < 0) {
        printf("Fork failed.\n");
        return -1;
    } else if (pid == 0) {
        int rc = execle("apue8_10environ", "apue8_10environ", "hello", "123", (char*)0, env);
        if (rc == -1) {
            printf("execle failed.\n");
            return -1;
        }
    }

    return 0;
}
