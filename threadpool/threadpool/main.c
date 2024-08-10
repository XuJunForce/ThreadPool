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
    
    for (int i = 0; i < 100; ++i)
    {    
        int* num = (int*)malloc(sizeof(int));
        *num = i + 102;
        Task_add(pool, taskFunc, num);
    }



    sleep(30);

    return 0;
}

