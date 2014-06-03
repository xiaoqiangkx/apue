#include <limits.h>
#include <stdio.h>
#include <sys/resource.h>
#include <unistd.h>

int main() {
    struct rlimit rlim;
    
    long before = sysconf(_SC_CHILD_MAX);

    getrlimit(RLIMIT_NPROC, &rlim);
    rlim.rlim_cur = 1;
    setrlimit(RLIMIT_NPROC, &rlim);

    long after = sysconf(_SC_CHILD_MAX);
    printf("before=%ld, after=%ld\n", before, after);

    if (rlim.rlim_max == RLIM_INFINITY) {
        printf("no limit\n");
    } else {
        printf("limit is %ld\n", rlim.rlim_max);
    }

    return 0;
}

