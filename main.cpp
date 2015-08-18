#include "threadPool.h"

int main()
{
	ThreadPool &inst= ThreadPool::Getinstance();
	inst.InitThreadPool(2,8);

	WorkTask *pt= new WorkTask;

	pt->m_sTaskName= "linyourong";
	
	PInfo pi= new Info;
	pi->age= 24;
	pi->score= 100;
	pt->m_pTaskArg= pi;

	inst.AddTaskToThreadPool(pt);

	while(1)
	{
		sleep(60*100);
	}
	return 1;	
}
