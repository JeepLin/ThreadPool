#ifndef __THREAD_POOL_H
#define __THREAD_POOL_H

#include <iostream>
#include <list>
#include <pthread.h>
#include <string.h>

#define ADJUST_PTHREAD_NUM 2
#define MAX_TASK_NUM 2

typedef unsigned int UINT;

typedef struct _Info
{
	int age;
	int score;
}Info, * PInfo;

class Task
{
public:
	std::string m_sTaskName;
	void *m_pTaskArg;
	virtual bool run()= 0;
};

class WorkTask: public Task
{
	virtual bool run();
};

class ThreadPool
{
private:
	UINT m_uMax;
	UINT m_uMin;
	std::list<pthread_t> m_busyList;
	std::list<pthread_t> m_notBusyList;
	
	std::list<Task *>m_taskList;
	pthread_mutex_t m_mutexTaskList;
	pthread_mutex_t m_mutexThreadList;

	pthread_cond_t m_condTaskList;

	ThreadPool()
	{
		m_uMax= 0;
		m_uMin= 0;
	}

public:
	static ThreadPool & Getinstance()
	{
		static ThreadPool inst;
		return inst;
	}

	void InitThreadPool(UINT umin,UINT umax);

	virtual ~ThreadPool(){}
	
	friend void *pthreadFunctions(void *);
	friend void *pthreadManageFunctions(void *);
	bool AddTaskToThreadPool(Task *);
};

#endif

