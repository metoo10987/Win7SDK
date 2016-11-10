#pragma once
#include "semaphore_impl.h"
class rw_semaphore
{
public:
	rw_semaphore(string processname);
	~rw_semaphore(void);

	//等待可写信号
	void write_wait();

	//等待可读信号
	void read_wait();

	//设置写信号
	void write_set();

	//设置读信号
	void read_set();

	void close_semaphore();
private:
	semaphore_impl m_write;
	semaphore_impl m_read;
};
