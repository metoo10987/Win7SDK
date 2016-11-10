#pragma once

#ifndef LOOP_MESSAGE_QUEUE_MAX_NUM
	#define  LOOP_MESSAGE_QUEUE_MAX_NUM 10000
#endif	//LOOP_MESSAGE_QUEUE_MAX_NUM

#include <io/complate_port/io_complate_port.h>

#include <functional>
#include <vector>

namespace services
{
	VOID NTAPI server_work(PTP_CALLBACK_INSTANCE instance,PVOID context);
	class io_server
	{
	public:
		friend VOID NTAPI server_work(PTP_CALLBACK_INSTANCE instance,PVOID context);
		io_server(void);
		~io_server(void);
	private:
		typedef struct  _server_cmd
		{
			_server_cmd() : status(false){}
			~_server_cmd(){}

			typedef std::tr1::function<void(std::string)>* cmd_type;
			typedef bool status_type;
			typedef std::string parameter_type;

			cmd_type cmd;
			parameter_type param;
			status_type status;			//状态，false表示可写，true表示可读
		} server_cmd,*l_server_cmd;
		typedef server_cmd::cmd_type cmd_type;

	public:
		bool post_cmd(cmd_type cmd,std::string in_param)
		{
			return do_post_cmd(cmd,in_param);
		}

		void run(int thread_count = 2)
		{
			m_complate_port.create_port(thread_count);
			init_loop_message_queue();
			init_thread_pool(thread_count*2);
		}

	private:
		int m_write_index;
		int m_read_index;
		io_complate_port m_complate_port;
		std::vector<l_server_cmd> m_loop_message_queue;		//消息队列
		CRITICAL_SECTION m_r_loop_message_queue_cs;


		void init_loop_message_queue()
		{
			for(int i = 0; i < LOOP_MESSAGE_QUEUE_MAX_NUM; ++i)
				m_loop_message_queue.push_back(new server_cmd());
		}

		void init_thread_pool(int thread_count)
		{
			for(int i = 0; i < thread_count; ++i)
			{
				TrySubmitThreadpoolCallback(server_work,this,NULL);
			}
		}
		std::vector<l_server_cmd> copy_message_from_queue()
		{
			::EnterCriticalSection(&m_r_loop_message_queue_cs);
			std::vector<l_server_cmd> message_queue;
			int last_index = m_write_index;
			if(m_read_index != last_index)
			{
				if(m_read_index < last_index)
				{	//在同一次的循环上
					do_copy(m_read_index, last_index,message_queue);
				}else
				{	//不在同一次的循环上
					do_copy(m_read_index,LOOP_MESSAGE_QUEUE_MAX_NUM,message_queue);
					do_copy(0,last_index,message_queue);
				}
				m_read_index = last_index;
			}
			::LeaveCriticalSection(&m_r_loop_message_queue_cs);
			return message_queue;
		}

		void do_copy(int first, int last, std::vector<l_server_cmd>& dest_message_queue)
		{
			for(int i = first;i < last; ++i)
			{
				dest_message_queue.push_back(m_loop_message_queue.at(i));
				m_loop_message_queue.at(i)->status = false;
			}
		}

		bool do_post_cmd(cmd_type cmd,std::string in_param)
		{	//非线程安全
			bool result = false;
			if(m_write_index < LOOP_MESSAGE_QUEUE_MAX_NUM)
			{
				if( !m_loop_message_queue.at(m_write_index)->status)
				{
					m_loop_message_queue.at(m_write_index)->cmd = cmd;
					m_loop_message_queue.at(m_write_index)->param = in_param;
					m_loop_message_queue.at(m_write_index)->status = true;
					++m_write_index;
					result = true;
				}
			}else
			{
				m_write_index = 0;
				if( !m_loop_message_queue.at(m_write_index)->status)
				{
					m_loop_message_queue.at(m_write_index)->cmd = cmd;
					m_loop_message_queue.at(m_write_index)->param = in_param;
					m_loop_message_queue.at(m_write_index)->status = true;
					++m_write_index;
					result = true;
				}
			}
			if(result)
			{
				DWORD translate_num_bytes =0;
				ULONG_PTR complate_key = 0;
				m_complate_port.post_complate_status(translate_num_bytes,complate_key,NULL);
			}
			return result;
		}
	};

	typedef io_server server;
	inline VOID NTAPI server_work(PTP_CALLBACK_INSTANCE instance,PVOID context)
	{
		server* p_server = static_cast<server*>(context);
		for(;;)
		{
			if(p_server->m_complate_port.wait_complate_status())
			{
				std::vector<server::l_server_cmd> cmdlist = p_server->copy_message_from_queue();
				int n_count = cmdlist.size();
				for(int i = 0; i < n_count; ++i)
					(*(cmdlist[i]->cmd))(cmdlist[i]->param);
			}
		}
	}
};