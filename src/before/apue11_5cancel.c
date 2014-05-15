#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

void show1(void* str);

void *prt_fun (void* arg);

int main() {
    pthread_t npid;
    void *tret;
    pthread_create(&npid, NULL, prt_fun, NULL);

    sleep(1);
    pthread_cancel(npid);
    pthread_join(npid, &tret);
    printf("result is %d\n", (int)tret);
    return 0;
}


void show1(void* str) {
    printf("show data=%s\n", (char*)str);
}

void *prt_fun (void* arg) {
    pthread_cleanup_push(show1, "push1");
    pthread_cleanup_push(show1, "push2");
    printf("start thread\n");
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    sleep(2);
    return ((void*)1);
}
