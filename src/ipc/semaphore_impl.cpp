#include "semaphore_impl.h"


semaphore_impl::semaphore_impl(int usercount, int maxcount,string name)
{
	m_hSemaphore = CreateSemaphore(NULL,usercount,maxcount,name.c_str());
	if(NULL == m_hSemaphore)
	{
		throw "信号量创建失败！";
	}
}


semaphore_impl::~semaphore_impl(void)
{
	CloseImpl();
}

void semaphore_impl::SetImpl( int n /*= 1*/ )
{
	ReleaseSemaphore(m_hSemaphore,n,NULL);
}

void semaphore_impl::WaitImpl( long lMilliseconds /*= INFINITE*/ )
{
	WaitForSingleObject(m_hSemaphore,lMilliseconds);
}

void semaphore_impl::CloseImpl()
{
	CloseHandle(m_hSemaphore);
}
