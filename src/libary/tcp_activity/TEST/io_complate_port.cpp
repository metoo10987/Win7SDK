#include "io_complate_port.h"
using namespace services;

io_complate_port::io_complate_port(int thread_count/* = 2*/)
{
	if(!create_port(thread_count))
	{
		throw "error";
	}
}


io_complate_port::~io_complate_port(void)
{
}

bool services::io_complate_port::create_port( int thread_count /*= 2*/ )
{
	//���� IO ��ɶ˿ڣ�Ĭ��ͬһʱ������������2���߳�

	m_io_port = CreateIoCompletionPort(INVALID_HANDLE_VALUE,
		NULL,0,thread_count);
	return (m_io_port != NULL);
}

bool services::io_complate_port::associate_to_device( HANDLE h_device,complate_data_* complate_key )
{
	//���豸��������� IO ��ɶ˿�
	SOCKET s = (SOCKET)h_device;
	return (CreateIoCompletionPort((HANDLE)s,m_io_port,(ULONG_PTR)complate_key,4) == m_io_port);
}

bool services::io_complate_port::post_complate_status( DWORD translate_num_bytes ,ULONG_PTR complate_key, OVERLAPPED* p_overlapped /*= NULL*/ )
{
	//����һ��IO���״̬��IO��ɶ˿�

	return PostQueuedCompletionStatus(m_io_port,translate_num_bytes,
		complate_key,p_overlapped);
}

BOOL services::io_complate_port::wait_complate_status( 
	complate_data* &p_complete_data,
	complate_overlapped_* &p_complate_overlaped,
	DWORD out_time/* = 500*/ )
{
	//�ȴ�complate status��Ĭ�ϳ�ʱʱ��Ϊ500����
	DWORD translate_num_bytes = 0;
	complate_data* p_complete_data_T = NULL;
		complate_overlapped_* p_complate_overlaped_T = NULL;
	BOOL re =  GetQueuedCompletionStatus(m_io_port,(LPDWORD)&translate_num_bytes,
		(PULONG_PTR)&p_complete_data,(LPOVERLAPPED*)(&p_complate_overlaped),out_time);
	return re;
}
