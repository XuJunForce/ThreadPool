#include "threadpool.h"
void taskFunc(void* arg)
{
    int num = *(int*)arg;
    printf("thread %ld is working, number = %d\n",
        pthread_self(), num);
    sleep(1);
}

int main()
{
    // 创建线程池
    ThreadPool* pool = Thread_init(3, 10, 100);
    printf("线程池已创建\n");
    for (int i = 0; i < 100; ++i)
    {
        printf("%d个任务.........\n",i+1);
        int* num = (int*)malloc(sizeof(int));
        *num = i + 100;
        Task_add(pool, taskFunc, num);
    }

    sleep(30);

    return 0;
}

