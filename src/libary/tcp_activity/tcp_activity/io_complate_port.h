#pragma once

#include "init_sock.h"

#include <string>
using namespace std;


namespace services
{
	class io_complate_port
	{
	public:
		io_complate_port(int thread_count = 2);
		~io_complate_port(void);
		
		bool	associate_to_device(HANDLE h_device,ULONG_PTR p_complate_key);

		bool post_complate_status(DWORD translate_num_bytes ,ULONG_PTR complate_key,
			OVERLAPPED* p_overlapped = NULL);

		BOOL wait_complate_status(
			DWORD* p_translate_num,								//out 返回传输的字节数
			PULONG_PTR p_complate_key,							//out 返回与设备关联的 complate_key
			LPOVERLAPPED* p_p_complate_overlaped,		//out 返回提交的I/O
			DWORD out_time = 500);

	private:
		HANDLE m_io_port;
		int m_max_run_thread_count;			//可以同时运行的最大线程数量
	private:
		bool create_port(int thread_count = 2);
	};	
}

