#include <stdio.h>
#include "../dbg.h"
#include "alloc_path.h"
#include <unistd.h>

int main() {
    int size = 0;

    char *result = NULL;
    char *path_buf = path_alloc(&size);
    check(path_buf != NULL, "path_alloc return NULL");

    // TODO Need to deal with small length
    result = getcwd(path_buf, size);
    check(result != NULL, "failed to get working directory");
    printf("pwd is %s\n", path_buf);
    return 0;

error:
    return -1;
}
