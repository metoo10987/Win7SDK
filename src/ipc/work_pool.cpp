#include "work_pool.h"
#include <iostream>
using namespace std;

work_pool::work_pool(void)
{
}

work_pool::~work_pool(void)
{
}

DWORD WINAPI work_pool::thread_pool_callback( LPVOID pvContext )
{
	work_item* p_work_item = (work_item*)pvContext;
	(p_work_item->cmd)(p_work_item->in_param);
	delete p_work_item;
	return 0;
}

void work_pool::submit_work( cmdtype_1& cmd,any& obj)
{
	work_item* p_work_item = new work_item(cmd,obj);
	QueueUserWorkItem(
		work_pool::thread_pool_callback,
		(LPVOID)p_work_item,
		WT_EXECUTELONGFUNCTION);
}

void work_pool::close()
{
}

void work_pool::close_environment()
{
}

work_pool::work_item::work_item( cmdtype_1& cmd,any& obj )
{
	this->cmd = cmd;
	in_param = obj;
}

work_pool::work_item::~work_item()
{
}
