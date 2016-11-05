#pragma once
//进程间通信实现类
//该类实现两个进程间相互通信
#include <map>
#include <deque>
#include <functional>

#include "read_write_impl.h"
#include "work_pool.h"
#include "ipc_msg.h"

using namespace std;

class ipc_impl
{
public:
	typedef tr1::function<void(char* , int)> handle_type;
	typedef tr1::function<void(any&)> cmd_type;
	ipc_impl();
	ipc_impl(int buffer_size);

	~ipc_impl(void);

	//读模式连接到一块内存区域
	void listen(string name);

	void register_listen_handler(handle_type& handler);

	//写模式连接到一块内存区域
	void connect(string name);

	//向一块已经连接的内存写数据
	void write(string wname,const char* p_src, int langth);
	//关闭指定的连接

	void close(string name);

	//关闭所有连接
	void close_all();

private:
	static DWORD WINAPI ProcessReadThreadPro(LPVOID lpParameter);
private:
	read_write_impl* getnewCommunicate();
	void handle(any& msg);
	void submit_work(ipc_msg* p_msg);
	//初始化一个进程通信模块
	void InitWRCommunicate(string name,int buffersize,bool bRead = false);
private:
	map<string,read_write_impl*> m_pCommunicateSet;	//进程通信集合
	memory_map_file_impl m_ShareMemory;
	work_pool m_workhandle;

	deque<string> m_wait_deal;		//等待处理的读对象
	cmd_type m_cmd;
	handle_type m_handle;
	CRITICAL_SECTION m_cs;
	int m_buffer_size;
};

