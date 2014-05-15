#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include "../dbg.h"
using namespace std;

int main(int argc, char *argv[]) {
    string line;
    pid_t pid;
    int status = -1;    // 状态如何初始化

    cout << "%%  " << endl;
    while (getline(cin, line)) {
       if ((pid = fork()) < 0) {
          log_err("fork error");           
       } else if (0 == pid) {
           execlp(line.c_str(), line.c_str(), static_cast<char*>(0));
           log_err("couldn't execute: %s", line.c_str());
       }

       if ((pid = waitpid(pid, &status, 0)) < 0) {
           log_err("waitpid error");
       }

       cout << "%%  " << endl;
    }
    return 0;

error:
    return 127;
}
