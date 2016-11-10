#include "io_complate_port.h"
using namespace services;

io_complate_port::io_complate_port(void)
{
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

bool services::io_complate_port::associate_to_device( HANDLE h_device,ULONG_PTR complate_key )
{
	//将设备句柄关联到 IO 完成端口

	return (CreateIoCompletionPort(h_device,m_io_port,complate_key,4) == m_io_port);
}

bool services::io_complate_port::post_complate_status( DWORD translate_num_bytes ,ULONG_PTR complate_key, OVERLAPPED* p_overlapped /*= NULL*/ )
{
	//发送一个IO完成状态到IO完成端口

	return PostQueuedCompletionStatus(m_io_port,translate_num_bytes,
		complate_key,p_overlapped);
}

BOOL services::io_complate_port::wait_complate_status( 
	complate_overlapped_* p_complate_overlaped,
	DWORD out_time/* = 500*/ )
{
	//等待complate status，默认超时时间为500毫秒
	complate_data_* p_complete_key;
	ULONG_PTR complate_key;
	int translate_num_bytes;
	return GetQueuedCompletionStatus(m_io_port,&translate_num_bytes,
		&p_complete_key,(LPOVERLAPPED*)(&p_complate_overlaped),out_time);
}
