#pragma once
// �ͻ�������������
//	�������绷�����ȶ���Ҫ�����ݱ�����ȷ�ķ��͵�Ŀ���ַ�����Ҫ��Ŀ�����
//	Ӧ��ȷ�ϣ������ظ�����

#include "tcp_activity.h"
using namespace services;

class client_net_stream : public base_client_stream
{
public:
	typedef std::tr1::function<void(string)> cmd_type;
	typedef map<string, cmd_type> cmd_list_type;
#define MAKE_VALUE(x,y) cmd_list_type::value_type(x,y)

	client_net_stream(void);
	client_net_stream(unsigned long key);
	virtual ~client_net_stream(void);

	// I/O����
	virtual void handle_read(string recv_buf);
	virtual void handle_read_fail(int error_code);
	void to_send(string& mark, string& key, string& str);

	//	���ӣ�ɾ���¼�������
	void register_cmd(string key, cmd_type cmd);
	void delete_all_cmd();
	void delete_a_cmd(string key);

private:
	// �¼�����
	void deal_cmd(net_packet& packet);

	cmd_list_type m_cmd_list;
};
