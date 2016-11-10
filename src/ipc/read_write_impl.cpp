#include "read_write_impl.h"

read_write_impl::read_write_impl(string name,int buffersize):
m_semaphore(name),m_name(name),m_bEnd(false)
{
	m_buffer.CreateShareMemory(name+"_sharebuffer",buffersize);
}

read_write_impl::~read_write_impl(void)
{
}

void read_write_impl::write( const ipc_msg& msg )
{
	if(m_bEnd){
		return ;
	}else
	{
		m_semaphore.write_wait();
		m_buffer.WriteToShareMemory(msg);
		m_semaphore.read_set();
	}
}

ipc_msg* read_write_impl::read( )
{
	m_semaphore.read_wait();
	ipc_msg* p_msg = m_buffer.ReadFromShareMemory();
	m_semaphore.write_set();
	return p_msg;
}

bool read_write_impl::isEnd()
{
	return m_bEnd;
}

std::string read_write_impl::getname()
{
	return m_name;
}

void read_write_impl::close()
{
	m_bEnd = true;
	m_semaphore.read_set();
}
