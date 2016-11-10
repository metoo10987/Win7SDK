#pragma once
#include <string>
#include <WTypes.h>
#include <WinBase.h>

#include "ipc_msg.h"

using namespace std;
class memory_map_file_impl
{
public:
	memory_map_file_impl(void);
	~memory_map_file_impl(void);
public:
	HANDLE m_sharememoryhandle;
	void *m_pShareMemoryAddress;
	void CreateShareMemory(string name,size_t nSize);
	ipc_msg* ReadFromShareMemory();
	void WriteToShareMemory(const ipc_msg& msg);
private:
	size_t m_buffer_size;
};
