#include "memory_map_file_impl.h"
#include <Windows.h>

memory_map_file_impl::memory_map_file_impl(void)
{
}

memory_map_file_impl::~memory_map_file_impl(void)
{
}

void memory_map_file_impl::CreateShareMemory(string name, size_t nSize )
{
	m_sharememoryhandle = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,nSize,name.c_str());
	if(m_sharememoryhandle)
	{
		m_pShareMemoryAddress = MapViewOfFile(m_sharememoryhandle,FILE_MAP_ALL_ACCESS,0,0,0);
		if(m_pShareMemoryAddress == NULL)
		{
			throw "映射文件映射到调用进程地址时出错","错误";
		}
	}
	else
	{
		throw "创建文件映射失败","错误";
	}
	m_buffer_size = nSize;
}

ipc_msg* memory_map_file_impl::ReadFromShareMemory()
{
	ipc_msg* p_msg = new ipc_msg();
	memcpy((void*)p_msg->data(),m_pShareMemoryAddress,m_buffer_size);
	if(p_msg->decode())
		return p_msg;
	delete p_msg;
	return NULL;
}

void memory_map_file_impl::WriteToShareMemory( const ipc_msg& msg )
{
	memcpy(m_pShareMemoryAddress, (void*)(msg.data()), msg.length());
}
