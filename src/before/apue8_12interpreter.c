#include <unistd.h>
#include <stdio.h>

int main() {
    execlp("test.sh", "hello", (char*)0);
    return 0;
}
