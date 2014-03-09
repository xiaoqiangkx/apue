#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void* thr_fun(void*);

int main() {
    pthread_t ntid;
    int err = pthread_create(&ntid, NULL, thr_fun,NULL);

    if (err != 0) {
        printf("Failed to create thread\n");
        return -1;
    }
    
    char* data = NULL;
    pthread_join(ntid, (void**)(&data));
    printf("data= %s\n", data);
    free(data);
    return 0;
}

void* thr_fun(void* arg) {
    pthread_t i = pthread_self();
    printf("new thread:%d\n", (int)i);
    char *data = (char*)malloc(sizeof(char) * 6);
    data = strncpy(data, "hello", 5);
    pthread_exit(data);
    return NULL;
}
