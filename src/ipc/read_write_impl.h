#pragma once
//¶ÁÐ´Í¨ÐÅÄ£¿é
#include "rw_semaphore.h"
#include "memory_map_file_impl.h"
#include "ipc_msg.h"
class read_write_impl
{
public:
	read_write_impl(string name,int buffersize);
	~read_write_impl(void);
	void write(const ipc_msg& msg);
	ipc_msg* read();
	bool isEnd();
	string getname();
	void close();
private:
	string m_name;
	rw_semaphore m_semaphore;
	memory_map_file_impl m_buffer;
	bool m_bEnd;
};
