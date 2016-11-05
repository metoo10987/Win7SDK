#pragma once
#include "semaphore_impl.h"
class rw_semaphore
{
public:
	rw_semaphore(string processname);
	~rw_semaphore(void);

	//�ȴ���д�ź�
	void write_wait();

	//�ȴ��ɶ��ź�
	void read_wait();

	//����д�ź�
	void write_set();

	//���ö��ź�
	void read_set();

	void close_semaphore();
private:
	semaphore_impl m_write;
	semaphore_impl m_read;
};
