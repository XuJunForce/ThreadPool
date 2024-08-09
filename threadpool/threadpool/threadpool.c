#define _CRT_SECURE_NO_WARNINGS
#include "threadpool.h"

ThreadPool* thread_init(int min, int max, int capacity) {
    ThreadPool* threadPool = (ThreadPool*)malloc(sizeof(ThreadPool));

	do {
		threadPool->min = min;
		threadPool->max = max;
		threadPool->shutdown = 0;
        threadPool->tasksize = 0;
        threadPool->task=0;
        threadPool->taskTail=0;
        threadPool->task = (Task*)malloc(sizeof(Task) * threadPool->taskCapacity);
		if(pthread_mutex_init(&threadPool->poolMutex, NULL) !=0&&
		   pthread_cond_init(&threadPool->is_empty,NULL) != 0&&
		   pthread_cond_init(&threadPool->is_full, NULL) !=0)
		   {
			break;
		   }
		for(int i =0;i<min;i++){
			pthread_create(&threadPool->work[i],NULL,Worker,&threadPool);
		}
        pthread_create(&threadPool->manager,NULL, Manager,&threadPool);

		return threadPool;
	} while (0);
    if(threadPool && threadPool->task) free(threadPool->task);
    if(threadPool && threadPool->work) free(threadPool->work);
    if(threadPool && threadPool->manager) free(threadPool->manager);
    if(threadPool) free(threadPool);

    

	return NULL;




}


void* Worker(void* args){
    return NULL;
}


void* Manager(void* args){
    return NULL;



}