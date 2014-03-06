Advanced Programming in the Unix Environment Notes
==================================================

Learn Process, Thread, File System and IPC.

Contents
==========

* [Chapter 7 Process Environment](#chapter_7_process_environment)

Chapter 7 Process Environment
==============================

Design Idea
------------

**1. How does the main function is called?**  
**Answer**: A special routine which is set up by c compiler is called before the main function is called. The start-up routine takes value from the kernel-the command-line arguments and the environment, call exit and call handlers.

**2. How do the command-line argument are passed to the new program?**  
**Answer**:??

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

**7. What's the limits of a process?**

Shortcomming and Strength
-------------------------

Application
-----------

