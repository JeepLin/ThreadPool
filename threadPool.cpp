#include "threadPool.h"

//任务处理函数
bool WorkTask::run()
{
	std::cout << "student name: "<< m_sTaskName<< std::endl;
		
	PInfo info= (PInfo)m_pTaskArg;
	std::cout<< "student age: "<< info->age<< std::endl;
	std::cout<< "student score: "<< info->score<< std::endl;
	return true;
}

//线程池处理任务队列中任务函数
void * pthreadFunctions(void *args)
{
	ThreadPool *tp= (ThreadPool *)args;

	while(true)
	{
		std::cout<<"pthread id: "<<  pthread_self()<< std::endl;

		pthread_mutex_lock(&(tp->m_mutexTaskList));

		while(tp->m_taskList.size()== 0)
		{
			pthread_cond_wait(&(tp->m_condTaskList),&(tp->m_mutexTaskList));	
		}

		Task *iter= tp->m_taskList.front();
		tp->m_taskList.pop_front();

		pthread_mutex_unlock(&(tp->m_mutexTaskList));

		iter->run();	
	}
}

//线程池管理函数
void *pthreadManageFunctions(void *args)
{
	ThreadPool *tp= (ThreadPool *)args;

	while(true)
	{
		pthread_mutex_lock(&(tp->m_mutexThreadList));

		//没有任务时关闭线程
		if(tp->m_busyList.size()== 0 && tp->m_taskList.size()== 0)
		{
			for(int i= 0;i < ADJUST_PTHREAD_NUM;i++)
			{
				pthread_t pid=tp->m_notBusyList.front(); 
				pthread_cancel(pid);
				tp->m_notBusyList.pop_front();
			}
			pthread_mutex_unlock(&(tp->m_mutexThreadList));

		}	//任务量较多时创建线程
		else if((tp->m_notBusyList).size()== 0 && (tp->m_taskList).size() >= MAX_TASK_NUM)
		{
			pthread_t pid;
			pthread_mutex_unlock(&(tp->m_mutexThreadList));

			for(int i= 0;i < ADJUST_PTHREAD_NUM;i++)
			{
				int err= pthread_create(&pid,NULL,pthreadFunctions,tp);
	
				if(0!=err)
				{
					std::cout<< "pthrad_create error: "<< strerror(err)<< std::endl;
					continue;
				}
				pthread_join(pid,NULL);
			}
		}
		else
		{
			pthread_mutex_unlock(&(tp->m_mutexThreadList));
		}

		sleep(60*1000);
	}
}

//线程池初始化
void ThreadPool::InitThreadPool(UINT umin,UINT umax)
{
	int err;
	pthread_t pid;
	m_uMin= umin;
	m_uMax= umax;
	
	pthread_mutex_init(&m_mutexTaskList,NULL);
	pthread_mutex_init(&m_mutexThreadList,NULL);
	pthread_cond_init(&m_condTaskList,NULL);

	for(int i= 0;i< umin;i++)
	{
		err= pthread_create(&pid,NULL,pthreadFunctions,this);

		if(0!=err)
		{
			std::cout<< "pthrad_create error: "<< strerror(err)<< std::endl;
			continue;
		}

	}
	
	pthread_create(&pid,NULL,pthreadManageFunctions,this);
}

//增加任务节点至任务队列
bool ThreadPool::AddTaskToThreadPool(Task *ptask)
{
	if(ptask!= NULL)
	{
		pthread_mutex_lock(&m_mutexTaskList);
		m_taskList.push_back(ptask);
		pthread_mutex_unlock(&m_mutexTaskList);
	}
}

