第2章
多用`man`
1.1 more01: `read` lines from input , receive cmd, or quit
1.2 more02: fixes: receive cmd from `/dev/tty`
2.1 who1: `read` '/var/run/utmp' (deprecated in MacOS X)
3.1 cp: `create` dst, `read` from src file into buffer and dump buffer into src file

####小结：
1. `who`命令通过读系统日志的内容显示当前已经登录的用户。
2. Unix系统把数据存放在文件中，可以通过以下系统调用操作文件：
    - open(filename, how)
    - create(filename, mode)
    - read(fd, buf, max_buffersize)
    - write(fd, buf, real_buffersize) // 应注意检查返回值和`real_buffersize`是否相等
    - lseek(fd, dist, base)
    - close(fd)
3. 进程对文件的读写都要通过文件描述符，文件描述符表示文件和进程之间的连接
4. 每次系统调用都会导致用户模式和内核模式的切换以及执行内核代码，所以减少程序的系统调用可以提高程序运行效率
5. 程序可以通过缓冲技术减少系统调用次数，仅当写缓冲满或读缓冲空时才调用系统服务
6. Unix内核通过内核缓冲区减少IO次数
7. Unix中时间的处理方式是记录从某一时间开始经过的秒数(`time_t`类型)
8. 当系统调用出错时会把全局变量`errno`设置成响应的错误代码，并返回-1，程序可以通过检查`errno`来确定错误类型，并采取措施

#### Q&A
Q1. 如何确定数据已经被写到磁盘上，而不是被缓冲？
A1. 一般缓冲中的数据都有一个`dirty`标志。如果缓冲区的数据和磁盘中数据不一致，那么`dirty`位为1；反之，如果同步一致，将其清0。因此，刚写入缓冲区的数据默认`dirty`位为1,后面的同步操作会把这个位置0，表示被写到磁盘上。

Q2.标准IO与文件IO有什么区别？
A2. 标准IO(fopen, fgets, fgetc, fclose)的实现都包含了内核级的缓冲，使用`FILE`结构。本质上是标准C库函数，运行在用户态的高级IO，通常只能用来访问普通文件。
文件IO是不带缓冲的IO，属于系统调用，运行在内核态。用文件描述符表示打开的文件，可以访问不同类型的文件。
