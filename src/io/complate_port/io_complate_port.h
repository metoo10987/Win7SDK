#pragma once
#include <Windows.h>
#include "init_sock.h"


namespace services
{
	struct complate_overlapped_;
	typedef complate_overlapped_ complate_overlapped;
	typedef complate_overlapped_* pcomplate_overlapped;

	class io_complate_port
	{
	public:
		io_complate_port(void);
		~io_complate_port(void);
		bool create_port(int thread_count = 2);

		bool	associate_to_device(HANDLE h_device,ULONG_PTR complate_key);

		bool post_complate_status(DWORD translate_num_bytes ,ULONG_PTR complate_key,
			OVERLAPPED* p_overlapped = NULL);

		BOOL wait_complate_status( complate_overlapped_* p_complate_overlaped, DWORD out_time = 500);

		typedef struct complate_data_
		{
			int complate_key;
		} complate_data;
/*	private:*/
		HANDLE m_io_port;
	};

	//	∂®“Â OVERLAPPED 
	struct complate_overlapped_ : public OVERLAPPED
	{
		complate_overlapped_(char* p_buf, int len)
		{
			DataBuf.buf = p_buf;
			DataBuf.len = len;
		}
		int option_type;
		WSABUF DataBuf;
	};
}

