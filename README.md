Advanced Programming in the Unix Environment Notes
==================================================

Learn Process, Thread, File System and IPC.

Contents
==========

* [Chapter 7 Process Environment](#chapter_7_process_environment)
    * [Question](#question)
* [Chapter 8 Process Control](#chapter_8_process_control)

Chapter 7 Process Environment
==============================

Question
------------

**1. How does the main function is called?**

**Answer**: A special routine which is set up by c compiler is called before the main function is called. The start-up routine takes value from the kernel-the command-line arguments and the environment, call exit and call handlers.

**2. How do the command-line argument and environment variables are passed to the new program?**

**Answer**: start-up routine pas command-line argument. Environment variables can be got by invoking `getenv`.

> The Unix kernel never looks at environment variables; There interpretation is up to the various applications.

**Add or change Environment Variables**: Environment List is a ```char**``` list of pointers. If we modifying an existing name, we have to compare the length of the old one and new one. If we insert an name, we have to consirderthe first time and the second time. Use `malloc` and `realloc` to do the job.

**3. What does the typical memory layout looks like?**

**Answer**:  
1 Text segment: read only and sharable code.

2 Initialized data segment: integer/float/char data appearing outside any functions.

3 Uninitialized data segment: array/struct data appearing outside any functions.  

4 heap: alloc data

5 stack: function data. 1 stack for one function.

**Difference betwwen a.out and process segement**: a.out has symbol table, linkage tables which doesn't be loaded by process.
use ```$ size /usr/bin/cc /bin/sh``` to see the text\data\bss. 

**Shared library: Dynamic and Static**: Dynamic library save space and is easy to update while spend more time to run.

```bash
$ gcc -static hello.c
$ ls -l a.out
$ size a.out

$ gcc hello.c
$ ls -l a.out
$ size a.out
```

**4. How does the system allocate additional memory?**

**Answer**: 

`malloc`: data are uninitialized.

`alloc`: objects are initialized.

`realloc`: may move and copy data if space behind is not enough. Data between old ending and new ending are uninitialized.

`free`: data freed are returned to the malloc pool but return to kernel.

**record keeping**: space besides malloc data may be record keeping or other data.

**5. How many ways do the system terminate the program?**  

**Answer**: Return from main, calling `exit`, calling ```_exit``` or ```_Exit```;
Return of the last thread??```pthread_exit```??
Abort\signal\Response of the last thread to a cancellation request;

__Return from main__: The start-up routine call the main looks like:

```cpp
#include PATH-ENVIROMENT
exit(main(argc, argv));

```  

__Exit Handler__: use ```int atexit(void (*func)(void)); Return 0 if ok```, `exit()` will call handlers and cleanup standard I/O before call ```_Exit()```

**What will happen if no cleanup are done?**

**6. How do the longjmp and setjmp functions interact with the stack?**

**Answer**: we can not `goto` a label that is in another function. We should use `longjmp` and `setjmp` to perform this type of branching.

**Application**: If we encounter a nonfatal error, we do not want to exit the program. one ```setjmp(jmp_buf env)```, more ```longjmp(jmp_buf env, int val)```. `volatile` variable do not rolled back. `global` or `static` variable are stable which is not in stack. `register` will be rolled back and `automatic` variable will not. Use heap data.

**7. What's the limits of a process?**

**Answer**: memory, core file, cpu time in seconds, data segment, file, the number of file locks, the number of open files, the number of child processes, rss, socket buffer, stack.


Chapter 8 Process Control
==========================

**1. How to describe a process properly?**

**Answer**:  
1. A unique ID: delay use, which prevent a new process from being mistaken for the previous process to have used the same name; The id can be used to create a unique name for a file; Process ID 0 is the kernel process-swapper, ID 1 is the __init__ process which is invoked by the kernel at the end of bootstrap procedure. The __init__ process which is a user process with superuser privileges never die.

**2. What are the differences between parent and child process?**

**Answer**: 
* text segement is shared. Because of copy-on-write(COW) is used, they share the data space, stack and heap which is read-only. If one of them try to modify them, the child process get a copy.
* file buffer is shared as described in **src/apue8_3buffer.cpp**
* file table is copyed. The same __file offset__ let them append to the file. However we need to synchronization the process preventing write intermixly. We can close those descripters, too. 
* Real/Effective User/Group IDs, Process Group ID, CWD, File mask, Enviroment, resources limit. Shared memory segments.

**Fail to fork**:The number of process exceeds the sytem limit.

**When to use fork**: 1 Want to duplicate itself in order to process differentsection of code. 2 Execute a different program.(fork + exec == spawn)

**3. How to create and terminate a process?**

**Answer**:  
1. Fork: call `fork` once, return twice.
2. vfork: After we call `vfork`, the parent will not wake up until the child finish. They share the same space.
3. exit/_Exit: whether they call handler and clean up stdio.
4. terminate: closes all fds, release the memory. Process 1 `init` will be the parent of child who lost parent. The process is that, when a process terminate, the kernel will goes through all active processes to find their children and set thoses children parent to be 1. The kernel keep a small amout of information for every terminating process. `zombie` process is a terminated process whose parent forget to `wait` or `waitpid`. Because the parent of `zombie` will be closed. Init will close all its children which has a callback function to kill them.
5. wait or waitpid: if process has no child process, -1 is returned. waitpid provide non-block way.

**4. What is the race condition? How to cooperate between the parent and child process?**

**Answer**:  


**Tips**: setbuf(stdout, NULL) to set unbuffered.


**5. What are the interpreter files and system function?**


**6. How to start a process?**

**Answer**:
1. exec do not replace the fork id, but replace all spaces-text, data, heap and stack segment with a brand new program from the disk.
2. ```exec[l/v]e``` use full path with user-defined or system-defined path to exec. ```exec[l/v]p``` use user-defined path to exec data. 
