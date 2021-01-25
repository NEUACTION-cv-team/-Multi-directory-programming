#include "process_comm.h"

ProcessComm::ProcessComm():shm_addr(nullptr)
{

}

ProcessComm::~ProcessComm()
{

}

int ProcessComm::init(key_t key)
{ 
    // 使用约定的键值创建共享内存
    shmid = shmget((key_t) key,  BUF_SIZE, 0666 | IPC_CREAT);
    printf("shmid : %u\n", shmid);
    if (shmid < 0)
    {
        perror("shmget error!");
        return -1;
    }
 
    // 将共享内存附加到本进程
    shm_addr = shmat(shmid, NULL, 0);
    if (shm_addr == (void *) -1)
    {
        perror("shmat error!");
        return -1;
    }
 
	sharedLock = (pthread_mutex_t *)shm_addr;
    
    //互斥锁初始化
	pthread_mutexattr_init(&ma);
	pthread_mutexattr_setpshared(&ma, PTHREAD_PROCESS_SHARED);
	pthread_mutexattr_setrobust(&ma, PTHREAD_MUTEX_ROBUST);
	pthread_mutex_init(sharedLock,&ma);
    
    return 0;
}

void ProcessComm::write(char* buffer, int offset)
{
    while(pthread_mutex_lock(sharedLock) != 0);     //获取互斥锁　不成功一直等待（可加计时时强制退出）

    printf("send data: %s\n", buffer);
    memcpy(((pthread_mutex_t *)shm_addr)+1+offset, buffer, strlen(buffer));
    pthread_mutex_unlock(sharedLock);
}

void ProcessComm::read(char* buffer, int offset)
{
    while(pthread_mutex_lock(sharedLock) != 0);     //获取互斥锁　不成功一直等待（可加计时强制退出）

    memcpy(buffer, ((pthread_mutex_t *)shm_addr)+1+offset, BUF_SIZE);
    printf("read from shared memory: %s\n", buffer);
    pthread_mutex_unlock(sharedLock);
}

int ProcessComm::release()
{
    // 分离
    if (shmdt(shm_addr) == -1)
    {
        printf("shmdt error!\n");
        return -1;
    }

    // 删除共享内存
    if (shmctl(shmid, IPC_RMID, 0) == -1)
    {
        printf("shmctl error!\n");
        exit(1);
    }

    return 0;
}
