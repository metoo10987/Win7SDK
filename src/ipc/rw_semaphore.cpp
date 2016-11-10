#include "rw_semaphore.h"

rw_semaphore::rw_semaphore(string processname):
m_write(1,1,processname+"write"),m_read(0,1,processname+"read")
{
}

rw_semaphore::~rw_semaphore(void)
{
}

void rw_semaphore::write_wait()
{
	m_write.WaitImpl();
}

void rw_semaphore::read_wait()
{
	m_read.WaitImpl();
}

void rw_semaphore::write_set()
{
	m_write.SetImpl();
}

void rw_semaphore::read_set()
{
	m_read.SetImpl();
}

void rw_semaphore::close_semaphore()
{
	m_write.CloseImpl();
	m_read.CloseImpl();
}
