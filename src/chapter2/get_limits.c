#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <climits>
using namespace std;

int main(int argc, char *argv[]) {
    #ifdef _POSIX_OPEN_MAX
        cout << "_POSIX_OPEN_MAX=" << _POSIX_OPEN_MAX << endl;
    #else
        cout << "no symbol for _POSIX_OPEN_MAX" << endl;
    #endif

    #ifdef OPEN_MAX
        cout << "OPEN_MAX=" << OPEN_MAX << endl;
    #else
        cout << "no symbol for OPEN_MAX" << endl;
    #endif

    errno = 0;  // reset to zero
    long result = -1;
    if (result = sysconf(_SC_OPEN_MAX), result < 0) {
        if (errno != 0) {
            if (errno == EINVAL) {
                cout << "not supported" << endl;
            } else {
                cout << "sys error" << endl;
            }
        } else {
            cout << "no limit" << endl;
        }
    } else {
        cout << "result=" << result << endl;
    }

    return 0;
}
