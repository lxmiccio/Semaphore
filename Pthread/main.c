#include <pthread.h>
#include <stdio.h>

struct __numbers {
        int x;
        int y;
};

void* ex1Print(void* i);
int ex1();

int ex2();

void* ex3Sum(void* numbers);
void* ex3Subtraction(void* numbers);
void* ex3Multiplication(void* numbers);
void* ex3Division(void* numbers);
int ex3();

int ex4();
int ex5();

void* ex6Sum(void* numbers);
void* ex6Subtraction(void* numbers);
void* ex6Multiplication(void* numbers);
void* ex6Division(void* numbers);
int ex6();

int main(int argc, char *argv[])
{
    //ex1();
    //ex2();
    //ex3();
    //ex4();
    //ex5();
    ex6();

    return 0;
}

void* ex1Print(void* i)
{
    int* x_ptr = (int *)i;
    printf("i is %d\n", *x_ptr);
    pthread_exit(NULL);
}

int ex1()
{
    pthread_t threads[4];

    for(int i = 0; i < 4; ++i)
    {
        if(pthread_create(&threads[i], NULL, ex1Print, &i))
        {
            fprintf(stderr, "Error creating thread\n");
            return 1;
        }
    }

    for(int i = 0; i < 4; ++i)
    {
        if(pthread_join(threads[i], NULL))
        {
            fprintf(stderr, "Error waiting thread\n");
            return 1;
        }
    }

    return 0;
}

int ex2()
{
    pthread_t threads[4];

    for(int i = 0; i < 4; ++i)
    {
        if(pthread_create(&threads[i], NULL, ex1Print, &i))
        {
            fprintf(stderr, "Error creating thread\n");
            return 1;
        }

        if(pthread_join(threads[i], NULL))
        {
            fprintf(stderr, "Error waiting thread\n");
            return 1;
        }
    }

    return 0;
}

void* ex3Sum(void* numbers)
{
    struct __numbers* num = (struct __numbers*) numbers;
    pthread_exit((void*) (num->x + num->y));
}

void* ex3Subtraction(void* numbers)
{
    struct __numbers* num = (struct __numbers*) numbers;
    pthread_exit((void*) (num->x - num->y));
}

void* ex3Multiplication(void* numbers)
{
    struct __numbers* num = (struct __numbers*) numbers;
    pthread_exit((void*) (num->x * num->y));
}

void* ex3Division(void* numbers)
{
    struct __numbers* num = (struct __numbers*) numbers;
    pthread_exit((void*) (num->x / num->y));
}

int ex3()
{
    struct __numbers num;
    num.x = 10;
    num.y = 5;

    pthread_t threads;
    if(pthread_create(&threads, NULL, ex3Sum, &num))
    {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    int* result = 0;
    int error = pthread_join(threads, (void*) &result);
    if(error)
    {
        fprintf(stderr, "Error waiting thread, error is %d\n", error);
        return 1;
    }
    else
    {
        printf("Result is %d\n", result);
    }

    return 1;
}

int ex4()
{
    struct __numbers num;
    printf("Enter number 1: ");
    scanf("%d", &num.x);
    printf("Enter number 2: ");
    scanf("%d", &num.y);

    pthread_t threads[4];
    void* functions[4];
    functions[0] = ex3Sum;
    functions[1] = ex3Subtraction;
    functions[2] = ex3Multiplication;
    functions[3] = ex3Division;

    for(int i = 0; i < 4; ++i)
    {
        if(pthread_create(&threads[i], NULL, functions[i], &num))
        {
            fprintf(stderr, "Error creating thread\n");
            return 1;
        }
    }

    for(int i = 0; i < 4; ++i)
    {
        int* result = 0;
        int error = pthread_join(threads[i], (void*) &result);
        if(error)
        {
            fprintf(stderr, "Error waiting thread, error is %d\n", error);
            return 1;
        }
        else
        {
            printf("Result of thread %d is %d\n", i + 1, *result);
        }
    }

    return 0;
}

int ex5()
{
    int operation = 0;

    struct __numbers num;
    printf("Enter number 1: ");
    scanf("%d", &num.x);
    printf("Enter number 2: ");
    scanf("%d", &num.y);
    printf("Enter operation (1 sum, 2 sub, 3 mul, 4 div): ");
    scanf("%d", &operation);

    void* function = NULL;
    switch(operation)
    {
        case 1:
            function = ex3Sum;
            break;
        case 2:
            function = ex3Subtraction;
            break;
        case 3:
            function = ex3Multiplication;
            break;
        case 4:
            function = ex3Division;
            break;
        default:
            fprintf(stderr, "Invalid choice!\n");
            return 1;
    }

    pthread_t threads;
    if(pthread_create(&threads, NULL, function, &num))
    {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    int* result = 0;
    int error = pthread_join(threads, (void*) &result);
    if(error)
    {
        fprintf(stderr, "Error waiting thread, error is %d\n", error);
        return 1;
    }
    else
    {
        printf("Result is %d\n", result);
    }

    return 0;
}

void* ex6Sum(void* numbers)
{
    struct __numbers* num = (struct __numbers*) numbers;
    num->x = num->x + num->y;
    pthread_exit((void*) num);
}

void* ex6Subtraction(void* numbers)
{
    struct __numbers* num = (struct __numbers*) numbers;
    num->x = num->x - num->y;
    pthread_exit((void*) num);
}

void* ex6Multiplication(void* numbers)
{
    struct __numbers* num = (struct __numbers*) numbers;
    num->x = num->x * num->y;
    pthread_exit((void*) num);
}

void* ex6Division(void* numbers)
{
    struct __numbers* num = (struct __numbers*) numbers;
    num->x = num->x / num->y;
    pthread_exit((void*) num);
}

int ex6()
{
    int operation = 0;

    struct __numbers num;
    printf("Enter number 1: ");
    scanf("%d", &num.x);
    printf("Enter number 2: ");
    scanf("%d", &num.y);
    printf("Enter operation (1 sum, 2 sub, 3 mul, 4 div): ");
    scanf("%d", &operation);

    void* function = NULL;
    switch(operation)
    {
        case 1:
            function = ex6Sum;
            break;
        case 2:
            function = ex6Subtraction;
            break;
        case 3:
            function = ex6Multiplication;
            break;
        case 4:
            function = ex6Division;
            break;
        default:
            fprintf(stderr, "Invalid choice!\n");
            return 1;
    }

    pthread_t threads;
    if(pthread_create(&threads, NULL, function, &num))
    {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    struct __numbers* result;
    int error = pthread_join(threads, (void*) &result);
    if(error)
    {
        fprintf(stderr, "Error waiting thread, error is %d\n", error);
        return 1;
    }
    else
    {
        printf("Result is %d\n", result->x);
    }

    return 0;
}
