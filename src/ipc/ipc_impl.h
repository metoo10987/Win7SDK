#pragma once
//���̼�ͨ��ʵ����
//����ʵ���������̼��໥ͨ��
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

	//��ģʽ���ӵ�һ���ڴ�����
	void listen(string name);

	void register_listen_handler(handle_type& handler);

	//дģʽ���ӵ�һ���ڴ�����
	void connect(string name);

	//��һ���Ѿ����ӵ��ڴ�д����
	void write(string wname,const char* p_src, int langth);
	//�ر�ָ��������

	void close(string name);

	//�ر���������
	void close_all();

private:
	static DWORD WINAPI ProcessReadThreadPro(LPVOID lpParameter);
private:
	read_write_impl* getnewCommunicate();
	void handle(any& msg);
	void submit_work(ipc_msg* p_msg);
	//��ʼ��һ������ͨ��ģ��
	void InitWRCommunicate(string name,int buffersize,bool bRead = false);
private:
	map<string,read_write_impl*> m_pCommunicateSet;	//����ͨ�ż���
	memory_map_file_impl m_ShareMemory;
	work_pool m_workhandle;

	deque<string> m_wait_deal;		//�ȴ�����Ķ�����
	cmd_type m_cmd;
	handle_type m_handle;
	CRITICAL_SECTION m_cs;
	int m_buffer_size;
};

