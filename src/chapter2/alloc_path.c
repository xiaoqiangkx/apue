#include <stdio.h>
#include <stdlib.h>
#include "../dbg.h"
#include <limits.h>
#include <errno.h>
#include <unistd.h>

#ifdef PATH_MAX
static int pathmax = PATH_MAX;
#else
static int pathmax = 0;
#endif

#define PATH_MAX_GUESS 1024

#define SUSV3 200112L

static long posix_version = 0;

char* path_alloc(int *sizep) {

    if (posix_version == 0) {
        posix_version = sysconf(_SC_VERSION);
    }

    if (pathmax == 0) {
        errno = 0;
        pathmax = pathconf("/", _PC_PATH_MAX);
        if (pathmax < 0) {
            if (errno != 0) {
                log_err("pathconf error for _PC_PATH_MAX");
            } else {
                pathmax = PATH_MAX_GUESS;
            }
        } else {
            pathmax++;
        }
    }

    int size = 0;
    if (posix_version < SUSV3) {
        size = pathmax + 1;
    } else {
        size = pathmax;
    }

    char *ptr = NULL;
    if (ptr = (char*)malloc(size), ptr == NULL) {
        log_err("malloc error for length=%d", size);
    }

    if (sizep != NULL) {
        *sizep = size;
    }

    return ptr;

error:
    if (ptr) free(ptr);
    return NULL;
}
