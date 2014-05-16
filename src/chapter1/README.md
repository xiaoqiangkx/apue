chapter1 Notes
================

Code List
============

 * fig1\_7.cpp
    * 演示命令解释器的基本原理. process spawn出新进程的过程.
 * fig1\_10.cpp
    * 通过signal方法设置某个信号为SIG\_IGN, SIG\_DFL或者handler函数来控制SIGNAL
 * time\_value.cpp
    * 运行`$ make time\_value; time time\_value \< test.data`, 分析系统时间,用户时间和应用时间. (clock time是指单个程序运行时总共所需运行时间)

易犯错误
===========

`if ((pid == fork()) < 0)`
