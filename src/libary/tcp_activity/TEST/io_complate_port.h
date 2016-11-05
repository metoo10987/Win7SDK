#pragma once

#include "init_sock.h"


namespace services
{
	struct complate_overlapped_;
	typedef complate_overlapped_ complate_overlapped;
	typedef complate_overlapped_* pcomplate_overlapped;

	typedef struct complate_data_
	{
		int complate_key;
		SOCKET s;
	} complate_data;

	struct complate_overlapped_ : public OVERLAPPED
	{
		complate_overlapped_(char* p_buf, int len)
		{
			DataBuf.buf = p_buf;
			DataBuf.len = len;
		}
		complate_overlapped_(){}
		int option_type;
		WSABUF DataBuf;
	};

	class io_complate_port
	{
	public:
		io_complate_port(int thread_count = 2);
		~io_complate_port(void);
		
		bool	associate_to_device(HANDLE h_device,complate_data_* complate_key);

		bool post_complate_status(DWORD translate_num_bytes ,ULONG_PTR complate_key,
			OVERLAPPED* p_overlapped = NULL);

		BOOL wait_complate_status(complate_data* &p_complete_data, complate_overlapped_* &p_complate_overlaped, DWORD out_time = 500);

/*	private:*/
		HANDLE m_io_port;
	private:
		bool create_port(int thread_count = 2);
	};	
}

