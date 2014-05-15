chapter1 Notes
================

Code List
============

 * fig1\_7.cpp
    * 演示命令解释器的基本原理. process spawn出新进程的过程.
 * fig1\_10.cpp
    * 通过signal方法设置某个信号为SIG\_IGN, SIG\_DFL或者handler函数来控制SIGNAL

易犯错误
===========

`if ((pid == fork()) < 0)`
