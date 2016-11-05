#include "io_complate_port.h"
using namespace services;

io_complate_port::io_complate_port(int thread_count/* = 2*/):
m_max_run_thread_count(thread_count)
{
	create_port(m_max_run_thread_count);
}


io_complate_port::~io_complate_port(void)
{
}

bool services::io_complate_port::create_port( int thread_count /*= 2*/ )
{
	//创建 IO 完成端口，默认同一时间最多可以运行2个线程

	m_io_port = CreateIoCompletionPort(INVALID_HANDLE_VALUE,
		NULL,0,thread_count);
	return (m_io_port != NULL);
}

bool services::io_complate_port::associate_to_device( HANDLE h_device,ULONG_PTR p_complate_key )
{
	//将设备句柄关联到 IO 完成端口
	return (CreateIoCompletionPort(h_device,m_io_port,(ULONG_PTR)p_complate_key,m_max_run_thread_count) == m_io_port);
}

bool services::io_complate_port::post_complate_status( DWORD translate_num_bytes ,ULONG_PTR complate_key, OVERLAPPED* p_overlapped /*= NULL*/ )
{
	//发送一个IO完成状态到IO完成端口
	return PostQueuedCompletionStatus(m_io_port,translate_num_bytes,
		complate_key,p_overlapped);
}

BOOL services::io_complate_port::wait_complate_status(
	DWORD* p_translate_num,
	PULONG_PTR p_complate_key,LPOVERLAPPED* p_p_complate_overlaped,
	DWORD out_time/* = 500*/ )
{
	//等待complate status，默认超时时间为500毫秒
	bool flag = GetQueuedCompletionStatus(m_io_port,p_translate_num,
		p_complate_key,p_p_complate_overlaped,out_time);
	return flag;
}
