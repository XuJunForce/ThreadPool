#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

typedef struct Task {

}Task;

typedef struct ThreadPool {
	int min;	//线程池中最小的线程数
	int max;	//线程池中最小的线程数
	Task* task;		//任务队列
	int taskCapacity;	//最大容纳任务数量
	int tasksize;	//当前任务数量

	pthread_t* work;	//工作进程的ID
	pthread_t* manager;	//管理者的进程ID


	pthread_mutex_t poolMutex;	//线程池临界区
	pthread_cond_t is_empty;	//任务队列空
	pthread_cond_t is_full;		//任务队列满



}ThreadPool;


ThreadPool* thread_init(int min, int max, int capacity) {
	




}
