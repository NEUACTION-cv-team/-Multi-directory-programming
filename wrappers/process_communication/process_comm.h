#ifndef PROCESS_COMM_H_
#define PROCESS_COMM_H_

#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <errno.h>
#include <pthread.h>
 
#define BUF_SIZE 1024                   //my birthday
#define CV_ID       (key_t(1024))
#define LIDAR_ID    (key_t(2048))
#define MC_ID       (key_t(4096))

class ProcessComm
{
private:
    //共享内存标识符
    int shmid;
    //指向共享内存第一个字节的指针
    void *shm_addr;
	pthread_mutex_t * sharedLock;
	pthread_mutexattr_t ma;

public:
    ProcessComm();
    ~ProcessComm();
    int init(key_t key);
    void write(char* buffer, int offset = 0);
    void read(char* buffer, int offset = 0);
    int release();
};

#endif