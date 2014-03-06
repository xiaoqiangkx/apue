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

Shortcomming and Strength
-------------------------

Application
-----------

