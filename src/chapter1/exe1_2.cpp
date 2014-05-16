#include <iostream>
#include <unistd.h>
using namespace std;

int main(int argc, char *argv[]) {
    cout << "current process id is " << getpid() << endl;
    return 0;
}
