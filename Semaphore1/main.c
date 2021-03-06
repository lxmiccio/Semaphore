#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

union semun {
        int              value;
        struct semid_ds* buffer;
        unsigned short*  array;
};

void ex1();
void ex1_2();
void ex2();
void ex2_2();
void ex3();
void ex3_1();
void ex6();
void ex10();
void ex12(int argc, char *argv[]);
void ex13(int argc, char *argv[]);
void ex14(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    srand(time(NULL));

    //ex1();
    //ex1_2();
    //ex2();
    //ex2_2();
    //ex3();
    //ex3_1();
    //ex6();
    //ex10();
    //ex12(argc, argv);
    //ex13(argc, argv);
    ex14(argc, argv);

    return 0;
}

int waitSem(int semid)
{
    struct sembuf wait_buf;
    wait_buf.sem_num = 0;
    wait_buf.sem_op = -1;

    return semop(semid, &wait_buf, 1);
}

int waitSems(int semid, int semNumber)
{
    //printf("SemNumber is %d\n", semNumber);
    struct sembuf wait_buf;
    wait_buf.sem_num = semNumber;
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

int signalSems(int semid, int semNumber)
{
    struct sembuf signal_buf;
    signal_buf.sem_num = semNumber;
    signal_buf.sem_op = 1;

    return semop(semid, &signal_buf, 1);
}

void ex1()
{
    key_t key = ftok("/home/user/SistemiOperativiSemaphore/semaphore", rand());
    int semaphoreId = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
    if(semaphoreId == -1)
    {
        perror("semget failed!!!\n\n");
    }
    else
    {
        union semun arg;
        arg.value = 0; //mutex se =1

        int error = semctl(semaphoreId, 0, SETVAL, arg);
        if(error == -1)
        {
            perror("semctl failed!!!\n\n");
        }
        else
        {
            pid_t pid = fork();
            if(pid == -1)
            {
            }
            else if(pid == 0)
            {
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
                }

                exit(EXIT_SUCCESS);
            }
            else
            {
                if(waitSem(semaphoreId) == -1)
                {
                    perror("semop failed!!!\n\n");
                }
                else
                {
                    printf("Hi! I am the parent process %d\n", getpid());

                    if(signalSem(semaphoreId) == -1)
                    {
                        perror("semop failed!!!\n\n");
                    }
                }

                wait(NULL);
            }
        }
    }
}

void ex1_2()
{
    int id = rand();
    key_t key = ftok("/home/user/SistemiOperativiSemaphore/semaphore", id);
    int semaphoreId = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
    if(semaphoreId == -1)
    {
        perror("semget failed!!!\n\n");
    }
    else
    {
        union semun arg;
        arg.value = 0; //mutex se =1

        int error = semctl(semaphoreId, 0, SETVAL, arg);
        if(error == -1)
        {
            perror("semctl failed!!!\n\n");
        }
        else
        {
            pid_t pid = fork();
            if(pid == -1)
            {
            }
            else if(pid == 0)
            {
                //execl("/usr/bin/SistemiOperativiSemaphore2", "SistemiOperativiSemaphore2", itoa(id), NULL);
                printf("errno is %d", errno);
                perror("execl failed!!!\n\n");
            }
            else
            {
                //                if(waitSem(semaphoreId) == -1)
                //                {
                //                    perror("semop failed!!!\n\n");
                //                }
                //                else
                //                {
                printf("Hi! I am the parent process %d\n", getpid());

                if(signalSem(semaphoreId) == -1)
                {
                    perror("semop failed!!!\n\n");
                }
                //                }

                wait(NULL);
            }
        }
    }
}

void ex2()
{
    char buffer[4];

    key_t key = ftok("/home/user/SistemiOperativiSemaphore/god", 42);
    if(key == -1)
    {
        perror("ftok failed!!!");
        exit(EXIT_FAILURE);
    }

    int shmid = shmget(key, sizeof(buffer), IPC_CREAT | 0666);
    if(shmid == -1)
    {
        perror("shmget failed!!!");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork failed!!!");
        exit(EXIT_FAILURE);
    }
    else
    {
        if(pid == 0)
        {
            printf("Child\n");

            char* pointer = (char*) shmat(shmid, NULL, 0);
            if(pointer == NULL)
            {
                perror("shmat failed!!!");
                exit(EXIT_FAILURE);
            }

            strncpy(pointer, "abcd", 4);

            int error = shmdt(pointer);
            if(error == -1)
            {
                perror("shmdt failed!!!");
                exit(EXIT_FAILURE);
            }

            exit(EXIT_SUCCESS);
        }
        else
        {
            char* pointer = (char*) shmat(shmid, NULL, SHM_RDONLY);
            if(pointer == NULL)
            {
                perror("shmat failed!!!");
                exit(EXIT_FAILURE);
            }

            wait(NULL);
            printf("Parent\n");

            strncpy(buffer, pointer, 4);

            int error = shmdt(pointer);
            if(error == -1)
            {
                perror("shmdt failed!!!");
                exit(EXIT_FAILURE);
            }

            shmctl(shmid, IPC_RMID, NULL);

            printf("Read string is %s\n", buffer);
        }
    }
}

void ex2_2()
{
    char buffer[4];

    key_t key = ftok("/home/user/SistemiOperativiSemaphore/memory", 42);
    if(key == -1)
    {
        perror("ftok failed!!!");
        exit(EXIT_FAILURE);
    }

    int shmid = shmget(key, sizeof(buffer), IPC_CREAT | 0666);
    if(shmid == -1)
    {
        perror("shmget failed!!!");
        exit(EXIT_FAILURE);
    }

    char* pointer = (char*) shmat(shmid, NULL, SHM_RDONLY);
    if(pointer == NULL)
    {
        perror("shmat failed!!!");
        exit(EXIT_FAILURE);
    }

    ///
    key_t semKey = ftok("/home/user/SistemiOperativiSemaphore/semaphore", 42);
    int semId = semget(semKey, 1, IPC_CREAT | 0666);
    if(semId == -1)
    {
        perror("semget failed!!!\n\n");
    }

    union semun arg;
    arg.value = 0;

    int error = semctl(semId, 0, SETVAL, arg);
    if(error == -1)
    {
        perror("semctl failed!!!\n\n");
    }

    waitSem(semId);
    ///


    strncpy(buffer, pointer, 4);

    error = shmdt(pointer);
    if(error == -1)
    {
        perror("shmdt failed!!!");
        exit(EXIT_FAILURE);
    }
    shmctl(shmid, IPC_RMID, 0);

    printf("Read string is %s\n", buffer);
}

void ex3()
{
    char buffer[4];

    key_t key = ftok("/home/user/SistemiOperativiSemaphore/memory", 42);
    if(key == -1)
    {
        perror("ftok failed!!!");
        exit(EXIT_FAILURE);
    }

    int shmid = shmget(key, sizeof(buffer), IPC_CREAT | 0666);
    if(shmid == -1)
    {
        perror("shmget failed!!!");
        exit(EXIT_FAILURE);
    }

    char* pointer = (char*) shmat(shmid, NULL, 0);
    if(pointer == NULL)
    {
        perror("shmat failed!!!");
        exit(EXIT_FAILURE);
    }

    strncpy(pointer, "16", 4);

    ///
    key_t semKey = ftok("/home/user/SistemiOperativiSemaphore/semaphore", 42);
    int semId = semget(semKey, 1, IPC_CREAT | 0666);
    if(semId == -1)
    {
        perror("semget failed!!!\n\n");
    }

    union semun arg;
    arg.value = 1;

    int error = semctl(semId, 0, SETVAL, arg);
    if(error == -1)
    {
        perror("semctl failed!!!\n\n");
    }
    ///

    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork failed!!!");
        exit(EXIT_FAILURE);
    }
    else
    {
        if(pid == 0)
        {
            printf("Child *2\n");

            waitSem(semId);
            int number = atoi(pointer);
            number *= 2;
            snprintf(pointer, 4, "%d", number);
            signalSem(semId);

            exit(EXIT_SUCCESS);
        }
        else
        {
            printf("Parent\n");

            pid = fork();
            if(pid == -1)
            {
                perror("fork failed!!!");
                exit(EXIT_FAILURE);
            }
            else
            {
                if(pid == 0)
                {
                    printf("Child +100\n");

                    waitSem(semId);
                    int number = atoi(pointer);
                    number += 100;
                    snprintf(pointer, 4, "%d", number);
                    signalSem(semId);

                    exit(EXIT_SUCCESS);
                }
                else
                {
                    while(wait(NULL) > 0);

                    printf("Parent\n");
                    printf("Number is %s\n", pointer);

                    int error = shmdt(pointer);
                    if(error == -1)
                    {
                        perror("shmdt failed!!!");
                        exit(EXIT_FAILURE);
                    }

                    printf("Shared memory destroyed!!!\n");
                }
            }
        }
    }
}

void ex3_1()
{
    key_t key = ftok("/home/user/SistemiOperativiSemaphore/memory", 42);
    if(key == -1)
    {
        perror("ftok failed!!!");
        exit(EXIT_FAILURE);
    }

    int shmId = shmget(key, sizeof(int), IPC_CREAT | 0666);
    if(shmId == -1)
    {
        perror("shmget failed!!!");
        exit(EXIT_FAILURE);
    }

    int* pointer = (int*) shmat(shmId, NULL, 0);
    if(pointer == NULL)
    {
        perror("shmat failed!!!");
        exit(EXIT_FAILURE);
    }

    *pointer = 16;

    ///
    key_t semKey = ftok("/home/user/SistemiOperativiSemaphore/semaphore", 42);
    int semId = semget(semKey, 1, IPC_CREAT | 0666);
    if(semId == -1)
    {
        perror("semget failed!!!\n\n");
    }

    union semun arg;
    arg.value = 1;

    int error = semctl(semId, 0, SETVAL, arg);
    if(error == -1)
    {
        perror("semctl failed!!!\n\n");
    }
    ///

    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork failed!!!");
        exit(EXIT_FAILURE);
    }
    else
    {
        if(pid == 0)
        {
            printf("Child *2\n");

            waitSem(semId);
            *pointer *= 2;
            signalSem(semId);

            exit(EXIT_SUCCESS);
        }
        else
        {
            pid = fork();
            if(pid == -1)
            {
                perror("fork failed!!!");
                exit(EXIT_FAILURE);
            }
            else
            {
                if(pid == 0)
                {
                    printf("Child +100\n");

                    waitSem(semId);
                    *pointer += 100;
                    signalSem(semId);

                    exit(EXIT_SUCCESS);
                }
                else
                {
                    while(wait(NULL) > 0);

                    printf("Parent\n");
                    printf("Number is %d\n", *pointer);

                    int error = shmdt(pointer);
                    if(error == -1)
                    {
                        perror("shmdt failed!!!");
                        exit(EXIT_FAILURE);
                    }

                    printf("Shared memory destroyed!!!\n");
                }
            }
        }
    }
}

void ex6()
{
    int n = 8;

    key_t key = ftok("/home/user/SistemiOperativiSemaphore/semaphore", 240);
    if(key == -1)
    {
        perror("ftok failed!!!");
        exit(EXIT_FAILURE);
    }

    //Max sem number is 32000
    int semId = semget(key, n, IPC_CREAT | 0666);
    if(semId == -1)
    {
        fprintf(stderr, "semget failed, errno is %d!!!", errno);
        exit(EXIT_FAILURE);
    }

    pid_t pids[n];

    for(int i = 0; i < n; ++i)
    {
        union semun arg;
        //arg.value = !i;

        if(i == 0)
        {
            arg.value = 1;
        }
        else
        {
            arg.value = 0;
        }

        semctl(semId, i, SETVAL, arg);

        pids[i] = fork();
        if(pids[i] < 0)
        {
            perror("fork failed!!!");
            exit(EXIT_FAILURE);
        }
        else if(pids[i] == 0)
        {
            waitSems(semId, i);
            printf("I am the %d-th process\n", i);
            signalSems(semId, i + 1);
            exit(EXIT_SUCCESS);
        }
    }

    while(wait(NULL) > 0);
}

typedef struct Msg
{
        long mtype;
        char mtext[1];
} Message;

void ex10()
{
    key_t key = ftok("/home/user/SistemiOperativiSemaphore/memory", 42);
    if(key == -1)
    {
        perror("ftok failed!!!");
    }

    int msgId = msgget(key, IPC_CREAT | 0666);
    if(msgId == -1)
    {
        perror("msgget failed!!!");
    }

    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork failed!!!");
    }
    else if(pid == 0)
    {
        Message msg;
        msg.mtype = 1;
        strncpy(msg.mtext, "1", 1);

        int i = 0;
        for(i = 0; i < 3; ++i)
        {
            int error = msgsnd(msgId, &msg, sizeof(msg.mtext), 0);
            if(error == -1)
            {
                perror("msgsnd failed!!!");
            }
            else
            {
                printf("Message sent\n");
            }
        }

        exit(EXIT_SUCCESS);
    }
    else
    {
        pid = fork();
        if(pid == -1)
        {
            perror("fork failed!!!");
        }
        else if(pid == 0)
        {
            Message msg;
            msg.mtype = 1;
            strncpy(msg.mtext, "2", 1);

            int i = 0;
            for(i = 0; i < 3; ++i)
            {
                int error = msgsnd(msgId, &msg, sizeof(msg.mtext), 0);
                if(error == -1)
                {
                    perror("msgsnd failed!!!");
                }
                else
                {
                    printf("Message sent\n");
                }
            }

            exit(EXIT_SUCCESS);
        }
        else
        {
            while(wait(NULL) > 0);

            Message msg;
            int i = 0;

            for(i = 0; i < 6; ++i)
            {
                int byteRead = msgrcv(msgId, &msg, sizeof(msg.mtext), 1, MSG_NOERROR);
                if(byteRead == -1)
                {
                    perror("msgrcv failed!!!");
                }
                else
                {
                    char text[10];
                    strncpy(text, msg.mtext, byteRead);
                    printf("Message content %s\n", text);
                }
            }

            int error = msgctl(msgId, IPC_RMID, NULL);
            if(error == -1)
            {
                perror("msgctl failed!!!");
            }

            printf("Tail destroyed!!!\n");
        }
    }
}

//ex12 semaphore
void ex12(int argc, char *argv[])
{
    DIR* dir = opendir(".");
    if(dir == NULL)
    {
        fprintf(stderr, "opendir() returned NULL, errno is %d!!!\n\n", errno);
        exit(EXIT_FAILURE);
    }
    else
    {
        struct dirent* entry = NULL;
        while((entry = readdir(dir)) != NULL)
        {
            for(int i = 1; i < argc; ++i)
            {
                if(!strcmp(entry->d_name, argv[i]))
                {
                    if(entry->d_type == DT_DIR)
                    {
                        printf("%s is a folder\n", argv[i]);
                    }
                    else if(entry->d_type == DT_REG)
                    {
                        printf("%s is a regular file\n", argv[i]);
                    }
                }
            }
        }
    }
}

//ex12 semaphore
void ex13(int argc, char *argv[])
{
    DIR* directory = opendir(".");
    if(directory == NULL)
    {
        fprintf(stderr, "opendir() returned NULL, errno is %d!!!\n\n", errno);
        exit(EXIT_FAILURE);
    }
    else
    {
        struct dirent* entry = NULL;
        while((entry = readdir(directory)) != NULL)
        {
            for(int i = 1; i < argc; ++i)
            {
                if(!strcmp(entry->d_name, argv[i]))
                {
                    if(entry->d_type == DT_DIR)
                    {
                        printf("%s is a folder\n", argv[i]);
                    }
                    else if(entry->d_type == DT_REG)
                    {
                        struct stat buffer;
                        int error = stat(entry->d_name, &buffer);
                        if(error == -1)
                        {
                            fprintf(stderr, "Cannot stat file %s!!!\n\n", entry->d_name);
                        }
                        else
                        {
                            printf("%s is a file, size is %ld bytes\n", argv[i], buffer.st_size);
                        }
                    }
                }
            }
        }

        int error = closedir(directory);
        if(error == -1)
        {
            fprintf(stderr, "Cannot close directory %s!!!\n\n", argv[1]);
            exit(EXIT_FAILURE);
        }
    }
}

void searchRecursive(char* dir, char* file)
{
    DIR* directory = opendir(dir);
    if(directory == NULL)
    {
        fprintf(stderr, "opendir() returned NULL, errno is %d!!!\n\n", errno);
        exit(EXIT_FAILURE);
    }
    else
    {
        struct dirent* entry = NULL;
        while((entry = readdir(directory)) != NULL)
        {
            if(!strcmp(entry->d_name, file))
            {
                printf("%s/%s is the file\n", dir, entry->d_name);
            }

            if(entry->d_type == DT_DIR && strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
            {
                char buffer[256];
                strncpy(buffer, "\0", 256);
                strcat(buffer, dir);
                strcat(buffer, "/");
                strcat(buffer, entry->d_name);

                searchRecursive(buffer, file);
            }
        }

        int error = closedir(directory);
        if(error == -1)
        {
            fprintf(stderr, "Cannot close directory %s!!!\n\n", dir);
            exit(EXIT_FAILURE);
        }
    }
}

void ex14(int argc, char *argv[])
{
    if(argc >= 3)
    {
        searchRecursive(argv[1], argv[2]);
    }
}
