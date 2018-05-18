#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

union semun {
    int              value;
    struct semid_ds* buffer;
    unsigned short*  array;
};

int waitSem(int semid);
int signalSem(int semid);

void ex1_1(int argc, char *argv[]);
void ex2_1();

int main(int argc, char *argv[])
{
    //ex1_1(argc, argv);
    ex2_1();
    return 0;
}

int waitSem(int semid)
{
    struct sembuf wait_buf;
    wait_buf.sem_num = 0;
    wait_buf.sem_op = -1;

    return semop(semid, &wait_buf, 1);
}

int signalSem(int semid)
{
    struct sembuf signal_buf;
    signal_buf.sem_num = 0;
    signal_buf.sem_op = 1;

    return semop(semid, &signal_buf, 1);
}

void ex1_1(int argc, char *argv[])
{
    key_t key = ftok("/home/user/SistemiOperativiSemaphore/filename.txt", atoi(argv[2]));
    int semaphoreId = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
    if(semaphoreId == -1)
    {
        if(errno == EEXIST)
        {
            semaphoreId = semget(key, 1, 0666);
        }
        else
        {
            perror("semget failed!!!\n\n");
        }
    }

//        union semun arg;
//        arg.value = 0; //mutex se =1

//        int error = semctl(semaphoreId, 0, SETVAL, arg);
//        if(error == -1)
//        {
//            perror("semctl failed!!!\n\n");
//        }
//        else
//        {
            sleep(5);

            if(waitSem(semaphoreId) == -1)
            {
                perror("semop failed!!!\n\n");
            }
            else
            {
                printf("Hi! I am the child process %d\n", getpid());

                if(signalSem(semaphoreId) == -1)
                {
                    perror("semop failed!!!\n\n");
                }
//            }
        }
}

void ex2_1()
{
    char buffer[4];

    key_t key = ftok("/home/user/SistemiOperativiSemaphore/memory", 42);
    if(key == -1)
    {
        perror("ftok failed!!!");
        exit(1);
    }

    int shmid = shmget(key, sizeof(buffer), IPC_CREAT | 0666);
    if(shmid == -1)
    {
        perror("shmget failed!!!");
        exit(1);
    }

    printf("Child\n");

    char* pointer = (char*) shmat(shmid, NULL, 0);
    if(pointer == NULL)
    {
        perror("shmat failed!!!");
        exit(1);
    }
    printf("Child\n");

    strncpy(pointer, "abcd", 4);
    printf("Child\n");

    int error = shmdt(pointer);
    if(error == -1)
    {
        perror("shmdt failed!!!");
        exit(1);
    }

    ///
    key_t semKey = ftok("/home/user/SistemiOperativiSemaphore/semaphore", 42);
    int semId = semget(semKey, 1, IPC_CREAT | 0666);
    if(semId == -1)
    {
        perror("semget failed!!!\n\n");
    }

//    union semun arg;
//    arg.value = 0;

//    error = semctl(semId, 0, SETVAL, arg);
//    if(error == -1)
//    {
//        perror("semctl failed!!!\n\n");
//    }

    signalSem(semId);
    ///
}
