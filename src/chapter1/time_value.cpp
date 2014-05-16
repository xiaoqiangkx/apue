#include <iostream>
#include <unistd.h>
using namespace std;

int main(int argc, char *argv[]) {
    const int BUF_SIZE = 10;
    char buf[BUF_SIZE];
    int n;
    while ((n = read(STDIN_FILENO, buf, BUF_SIZE-1)) > 0) {
        buf[n] = '\0';
        cout << buf << endl;
    }
    return 0;
}
