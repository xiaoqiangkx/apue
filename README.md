Advanced Programming in the Unix Environment Notes
==================================================

结合鸟哥的书, 理解常用的命令的实现逻辑.

Contents
==========

* [Chapter 1 Unix System Overview](#chapter_1_unix_system_overview)
* [Chpater 2 Unix Standardization and Implementations](#Chpater_2_Unix_Standardization_and_Implementations)
* [Chapter 3 File I0](#chapter_3_file_io)
* [Chapter 7 Process Environment](#chapter_7_process_environment)
    * [Question](#question)
* [Chapter 8 Process Control](#chapter_8_process_control)
* [Chapter 9 Process Relationship](#chapter_9_process_relationship)
* [Chapter 10 Signals](#chapter_10_signals)
* [Chapter 11 Threads](#chapter_11_threads)




本书介绍
===============

随着几十年的发展, Unix在非桌面应用领域, 特别是Android移动终端, 家用电器, 服务器等领域取得了飞速发展. 与此同时, AT&T鼓励在系统的接口和语言的基础上进行标准化的工作. 其发布的SVID成为了POSIX标准的基础. Linux和开源软件的崛起也对Unix生态圈改变颇大. 现在的四大主要系统是UNIX系统V版本后裔Sun的Solaris, 加州伯克利分校的FreeBSD, Linux以及Apple公司结合Mach和FreeBSD研发的Max 0S. 本书主要介绍这四个系统.

本书描述了系统调用接口和标准C库. 并且以贴近现实应用的形式说明了一些标准.

内容主要包括I/O, 进程(线程), IPC以及高级I/O.

**学习目的**   
希望通过这本书, 熟悉系统提供的资源以及系统的限制, 同时了解一些系统应用利用这些资源的方法和问题.


Chapter 1 Unix System Overview
================================

**登陆操作系统过程**: 输入账号和密码后, 系统查看`/etc/passwd`和`/etc/shadow`文件, 看看账号和密码是否正确; 如果正确那么查看`/etc/passwd`文件, 运行相应的shell程序,shell成为了我们和计算机之间的交互工具.

**配置文件解析**: 其中提及众多配置文件以及信息文件, 如`/etc/passwd`, 此文件使用**:**作为分隔符, 我们一般是如何获得第n个参数的呢? 一般是查找n-1和n个**:**之间的字符串, 使用两次`find`或者`strchr`, `strrchr`.

**问题1:**为什么文件系统需要把`.`目录, 只是为了方便查看pwd? `..`是为了指向parent实现回溯.


Chpater 2 Unix Standardization and Implementations
===================================================

**使用各系统的最小值从而换取兼容性**:系统的参数往往都有范围限制, 为了程序的兼容性, 程序假设的参数是取最小值. 如果某些程序限定在某个系统中运行,那么可以获取本系统允许的最大值.


如何获取标准定义的范围值以及本系统的最大值成为了我们探讨的范围? 
---------------------------------------------------------------
 
主要的标准是ISO C, POSIX和XSI

limits分为两大类:
 * compile-time limits(headers)
    * 变量字节数
 * Runtime limits:由于不同的文件系统或者系统运行状况下限制发生变化
    * file/directory无关,使用`sysconf`接口
    * file/directory有关,使用`pathconf`和`fpathconf`接口

ISO C中定义的所有limits都是compile-time限制.

**注**:由于int在16位和32位大小的机器上范围差异较大.涉及较大的数时,建议使用long或者一些posix变量.

POSIX标注定义了19个invariable变量,这是声称支持POSIX.1的系统必须兼容的.

**答**:`\_POSIX\_`开头的变量名,如`\_POSIX\_OPEN\_MAX`是标准定义的单进程最大fd数目,而去掉`\_POSIX\_`的变量名`OPEN\_MAX`是当前系统真实的支持大小.但是由于不是所有的系统都在limits中实现了这些变量. 所以POSIX.1定义了`sysconf`,`pathconf`和`fpathconf`接口来获得变量的真实值. 通常以`\_SC\_`和`\_PC\_`开头.

XSI首先定义了implementation limits, 如`NL\_ARGMAX`.

如何获得compile-limit和runtime-limit
---------------------------------------

使用`ifdef`宏和`sysconf`,`pathconf`,`fpathconf`方法.


如何处理indeterminate Runtime limits
--------------------------------------

**实例1:为pathname分配空间**

由于`\_POSIX\_PATH\_MAX`是standard limit, 其值太小没有利用价值. 必须获得implementation limit, 在`\<limits.h\>`中定义的`PATH\_MAX`或者`pathconf`中定义的limit具有使用价值, 但是当其值为indeterminate时,就只能guess了.

**细节1**: `pathconf`获得是相对路径最大长度, 计算绝对路径时需要加上当前的工作路径长度. 早期的系统获得的路径长度没有考虑null byte, 所以需要加1.

**细节2**: 在处理`getcwd`等需要传入path字节数组的函数时, 可能抛出ERANGE,数组长度国小的异常. 需要处理异常.

**细节3**: 系统define一个变量可能是0,表示需要使用sysconf等变量, 或者-1表示不支持, 或者大于0表示可以直接获得变量.

代码如下

见`chapter2/alloc\_path.c`



**实例2:获得进程能够打开的文件描述符数目**

由于`OPEN\_MAX`和`\_SC\_OPEN\_MAX`无法保证获得, 所以需要使用猜.

**注**: 不能依赖于`close()`方法的返回值,由于返回-1不一定是无效的fd,也有可能是未打开的fd.

**注**: 有些系统返回LONG\_MAX大小的`OPEN\_MAX`, 此时最好使用ulimit命令或者兼容XSI的getrlimit和setrlimit来改变上限.



Primitive System Data Type
-------------------------------

System Data Type的字节数是固定的, 不同的系统使用不同的C内置类型来实现. 如size\_t变量, 有些系统使用int, 有些使用long来实现, 从而实现源码跨平台.



Chapter 3 File I0
=====================

**限制**: 每个process可以打开的files是有限的, 一般由OPEN\_MAX或其它implementation limit限定.


名词解释
------------

 * unbuffered: 每次调用read和write都会引发系统调用.


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

**Answer**: 

1. when we input `ls -l`, the shell execute ```execlp("ls", "-l", (char*)NULL)```. The interpreter file let us choose other basical program other than `bin/sh`.
2. system let you just type the command other than use `execlp` function. It equals to `fork`, `exec("/bin/sh", "sh", "-c", cmdstring, ...)` and `waitpid`. It has a security hole. super-privilege program should never use `system` but use `fork` and `exec` to change permission in `fork`.


**6. How to start a process?**

**Answer**:
1. exec do not replace the fork id, but replace all spaces-text, data, heap and stack segment with a brand new program from the disk.
2. ```exec[l/v]e``` use full path with user-defined or system-defined path to exec. ```exec[l/v]p``` use user-defined path to exec data. 

**7. How to control privilege?**

**Answer:** Unix use __least_privilege__ model. Our program should use the least privilege necessary to accomplish any given task.

1. `setuid` and `setgid`, only root user can change real user id and set-user-ID. General user can change effective user id only when their real user ID or save-user-ID equals new id. The saved set-user-ID is copied from the effective user ID by exec. 


Chapter 9 Process Relationship
==============================

**1. How do terminal login?**

**Answer**: `init` fork `init` for every terminal, `init` exec `gettty` and `login`, `login` set up all the ids and PATH, later login reads start-up files, such as ".profile".


**2. What is Process Group and Session?**

**Answer**: 

1. Processes in a process group receive signals from the same terminal. The ID of Process Group Leader is the Process Group ID. Process can only set itself of its children before exec.
2. Session: A collection of one or more Process Group. No Session ID but Session Leader
3. Job Controller:

Chapter 10 Signals
===================


>  It is often important to understand what is wrong with an implementation before seeing how to do things correctly.


**What is Signals?**

**Answer**: Deals with asynchronous events. Denoted as an positive integer constant. number 0 is reserved. 

1. Disposition of the signal: ignore the signal(SIGKILL and SIGSTOP can not be stopped), catch the signal and let the default action apply(always terminate the program).
2. signal mask and disposition will be inheritanced.


Chapter 11 Threads
===================

**1. What is the difference between thread and process?**

**Answer**: 

1. Thread shares file descriptor and memory by default, but processhas to use complex mechanisms to realize it.
2. use pthread_t to indicate thread ID but integer. The thread ID is not unique in system but in process.

**2. What does a thread consist of?**

**Answer**: Thread ID, a stack, register values, a scheduling priority and policy, signal mask and an errno variable while global and heap memory, stacks(??) and the file descriptors are shared.

tips: use ```gcc file -E > tempfile``` to see result

**3. When do lock happen? Wen do the deadlock happen?**

**Answer**: When one thread modify a variable that other threads can read or modify it and thre modify operation can't be finished in one memory cycle which is not atomic. When there are many variables and they do not use variables in the same order, it lead to deadlock.

**4. What is Reader-Writer lock and condition lock?**

**Answer**: Reader-Writer lock can speed up read rates. condition lock can provide a place for threads to rendezvous.


关于头文件命名的一些问题
==========================

1. fork为什么在unistd.h里面?

参考资料
===========

* [Linux多线程调试](http://coolshell.cn/articles/3643.html)
