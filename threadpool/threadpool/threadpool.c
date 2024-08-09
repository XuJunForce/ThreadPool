#define _CRT_SECURE_NO_WARNINGS
#include "threadpool.h"

ThreadPool* Thread_init(int min, int max, int capacity) {
    ThreadPool* threadPool = (ThreadPool*)malloc(sizeof(ThreadPool));

	do {
		threadPool->min = min;
		threadPool->max = max;
        threadPool->numReduce=0;
		threadPool->shutdown = 0;
        threadPool->tasksize = 0;
        threadPool->task=0;
        threadPool->taskTail=0;
        threadPool->busyThread=0;
        threadPool->task = (Task*)malloc(sizeof(Task) * threadPool->taskCapacity);
        memset(threadPool->task,0,sizeof(Task));
        threadPool->work = (pthread_t*)malloc(sizeof(pthread_t)*max);
        memset(threadPool->work,0,sizeof(threadPool->work));
		if(pthread_mutex_init(&threadPool->poolMutex, NULL) !=0&&
		   pthread_cond_init(&threadPool->is_empty,NULL) != 0&&
		   pthread_cond_init(&threadPool->is_full, NULL) !=0)
		   {
			break;
		   }
		for(int i =0;i<min;i++){
			pthread_create(&threadPool->work[i],NULL,Worker,&threadPool);
		}
        threadPool->liveThread=min;
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
    ThreadPool* tp = (ThreadPool*)args;
    while(1){
        pthread_mutex_lock(&tp->poolMutex);    
        //任务空
        while(tp->taskHead==tp->taskTail){
            pthread_cond_wait(&tp->is_empty,&tp->poolMutex);
            //闲置线程太多需要被销毁
            while(tp->numReduce--){
                if(tp->liveThread > tp->min){
                    tp->liveThread--;
                    pthread_mutex_unlock(&tp->poolMutex);
                    Thread_exit(tp);
                }
            }
            pthread_mutex_unlock(&tp->poolMutex);
        }
        tp->busyThread++;
        void (*mission)(void*args) = tp->task->function;
        void* arg = tp->task->args;
        tp->taskHead = (tp->taskHead+1)%tp->taskCapacity;
        tp->tasksize--;
        pthread_cond_signal(&tp->is_full);
        pthread_mutex_unlock(&tp->poolMutex);

        mission(tp->task->args);


    }


    return NULL;
}


void* Manager(void* args){
    ThreadPool* tp = (ThreadPool*)args;
    while(1){
        pthread_mutex_lock(&tp->poolMutex);
        int busy = tp->busyThread;
        int live = tp->liveThread;
        int task = tp->tasksize;
        pthread_mutex_unlock(&tp->poolMutex);
        //如果忙线程 < 任务数 那么就加线程
        if(busy < task && live <tp->max){
            pthread_mutex_lock(&tp->poolMutex);
            for(int i = 0;(i<REDUCE)&&(live<tp->max);i++){
                for(int i =0;i<tp->max;i++){
                    if(tp->work[i]==0){
                        pthread_create(&tp->work[i],NULL, Worker, tp);
                        tp->liveThread++;
                        break;
                    }
                }
            }
            pthread_mutex_unlock(&tp->poolMutex);

        }



        //如果忙的线程*2 还小于存活的线程 那么就减少REDUCE个线程
        if(busy*2 < live){
            pthread_mutex_lock(&tp->poolMutex);
            tp->numReduce=2;
            for(int i = 0;i<REDUCE && live>tp->min;i++){
                pthread_cond_signal(&tp->is_empty);
            }
            pthread_mutex_unlock(&tp->poolMutex);
        }
        sleep(3);   //每3秒做一个检测
    }
    return NULL;
}







void Thread_exit(ThreadPool* tp){
    pthread_t id = pthread_self();
    for(int i = 0;i<tp->max;i++){
        if(id == tp->work[i]){
            tp->work[i] =0;
            break;
        }
    }
    pthread_exit(NULL);
}


void Task_add(ThreadPool* tp,void*(func)(void*),void*arg){
    pthread_mutex_lock(&tp->poolMutex);

    while(tp->tasksize>=tp->taskCapacity){
        pthread_cond_wait(&tp->is_full,&tp->poolMutex);
    }
    tp->task[tp->taskHead].function = func;
    tp->task[tp->taskHead].args = arg;
    tp->taskHead = (tp->taskHead++) % tp->taskCapacity;
    tp->tasksize++;
    pthread_cond_signal(&tp->is_empty);
    pthread_mutex_unlock(&tp->poolMutex);

}