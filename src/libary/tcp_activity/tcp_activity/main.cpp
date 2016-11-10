#include "tcp_activity.h"
#include <iostream>
using namespace services;


tcp_activity<> _tcp;
//tcp_activity<> _tcp_2;

DWORD WINAPI ThreadProc( LPVOID lpParameter)
{
	while(1)
	{
		DWORD trantlate_num = 0;
		_tcp.handle_io();
	}
	return 0;
}

void test(net_packet* p)
{
	string send_buf;
	send_buf.append(p->c_buffer,p->get_packet_length());
}

void test_1()
{
	string mark = "TCPTEST";
	int seral_num = 1;
	string key = "12345678901234567890123456789012";
	string str = "我是中国人，你很好";
	net_packet packet;
	packet.en_code(mark.c_str(), seral_num, key.c_str(),str.c_str(),str.size());
	return;
}
int main()
{


	int op_type;
	cout << "1 启动监听服务,2启动连接服务:";
	cin >> op_type;
	string ip = "192.168.1.66";
	if(op_type == 1)
	{
		_tcp.start_listen(4567);
		//_tcp_2.start_listen(5678);
	}
	else
	{
		_tcp.to_connect(ip,4567);
		//_tcp.to_connect(ip,5678);
	}
	int n;
	cin>>n;
	return 0;
}