#include "tcp_activity.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////	定义 base_tcp_stream	//////////////////////////////////////////////////////////
services::base_tcp_stream::base_tcp_stream( unsigned long key ) : m_key(key)
{
	InitializeCriticalSection(&send_cs);
}

services::base_tcp_stream::~base_tcp_stream()
{
	DeleteCriticalSection(&send_cs);
}

void services::base_tcp_stream::do_send( const string& str )
{
	EnterCriticalSection(&send_cs);
	string_separater<int> _separate(str,DATA_BUFSIZE);
	const char* p_buf = NULL;
	int len = 0;
	for(; _separate.separater_instance.get_next(&p_buf,len); )
		m_post_send_cdm(m_key,p_buf,len);
	if(len > 0)
		m_post_send_cdm(m_key,p_buf,len);
	LeaveCriticalSection(&send_cs);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////	定义 base_client_stream	///////////////////////////////////////////////////////
services::base_client_stream::base_client_stream()
{
}

services::base_client_stream::base_client_stream( unsigned long key ) : base_tcp_stream(key)
{
}

services::base_client_stream::~base_client_stream()
{
}

void services::base_client_stream::handle_read( string recv_buf )
{
	m_recvbuf += recv_buf;
	net_packet packet;
	int dealdata_length = 0;
	dealdata_length = packet.de_code(m_recvbuf.c_str(),m_recvbuf.size());
	if(dealdata_length == 0)
		return;
	for( ;dealdata_length > 0; )
	{
		++read_count;
		string content;
		int length = 0;
		char* p_content = packet.data(length);
		recv_bytes+=length;
		content.append(p_content,length);
		m_recvbuf.erase(m_recvbuf.begin(),m_recvbuf.begin()+dealdata_length);
		dealdata_length = packet.de_code(m_recvbuf.c_str(),m_recvbuf.size());
	}
}

void services::base_client_stream::handle_read_fail( int error_code )
{
	delete this;
}

void services::base_client_stream::send( net_packet* p_send_packet )
{
	string send_buf;
	send_buf.append(p_send_packet->c_buffer,p_send_packet->get_packet_length());
	do_send(send_buf);
}

void services::base_client_stream::set_socket_addr( SOCKET s )
{
	int len = sizeof (socket_addr);
	int n = getpeername(s ,(sockaddr*)&socket_addr,&len);
	if(n != 0)
	{
		int error_code = WSAGetLastError();
		cout << "错误"<<endl;
	}
}

std::string services::base_client_stream::get_remote_ip()
{
	return string(inet_ntoa(socket_addr.sin_addr));
}

int services::base_client_stream::get_remote_port()
{
	return ntohs(socket_addr.sin_port);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////	定义 TCP_OVERLAPPED	/////////////////////////////////////////////////////////
services::TCP_OVERLAPPED::TCP_OVERLAPPED()
{
	hEvent = WSACreateEvent();
	n_len = DATA_BUFSIZE;
	p_buff = new char[DATA_BUFSIZE+1];
	++ovelapped_count;
}

services::TCP_OVERLAPPED::~TCP_OVERLAPPED()
{
	WSACloseEvent(hEvent);
	delete []p_buff;
	--ovelapped_count;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////	定义 COMPLATE_KEY	////////////////////////////////////////////////////////////
services::COMPLATE_KEY::COMPLATE_KEY( SOCKET s )
{
	init_members();
	this->s = s;
}

services::COMPLATE_KEY::COMPLATE_KEY()
{
	init_members();
	s = INVALID_SOCKET;
}

services::COMPLATE_KEY::~COMPLATE_KEY()
{
	DeleteCriticalSection(&lock);
}

void services::COMPLATE_KEY::init_members()
{
	InitializeCriticalSection(&lock);
	++complate_key_count;
	current_sequnece = 0;
	read_sequence = 0;
	n_out_standing_recv_count = 0;
	n_out_standing_send_count = 0;
	b_closing = false;
	n = 0;
}

void services::COMPLATE_KEY::init_cs()
{
	InitializeCriticalSection(&lock);
}

void services::COMPLATE_KEY::release_rec()
{
	delete_connect((int)this);
}

void services::COMPLATE_KEY::handle_read( PTCP_OVERLAPPDE p_overlapped )
{
	do_recv_data(p_overlapped);
}

void services::COMPLATE_KEY::close()
{
	if(!b_closing)
	{
		closesocket(s);
		s = INVALID_SOCKET;
	}
	b_closing = true;
}

void services::COMPLATE_KEY::en_lock()
{
	EnterCriticalSection(&lock);
}

void services::COMPLATE_KEY::un_lock()
{
	LeaveCriticalSection(&lock);
}

int services::COMPLATE_KEY::get_pending_io_count()
{
	return n_out_standing_recv_count+n_out_standing_send_count;
}

void services::COMPLATE_KEY::push_recv_buf( PTCP_OVERLAPPDE p_overlapped )
{
	string data;
	data.append(p_overlapped->p_buff,p_overlapped->InternalHigh);
	no_order_reads.insert(map<unsigned long,string>::value_type(p_overlapped->n_sequence_number,data));
}

bool services::COMPLATE_KEY::get_next_data( string& str )
{
	map<unsigned long,string>::iterator it = no_order_reads.find(current_sequnece);
	if(it != no_order_reads.end())
	{
		str = it->second;
		no_order_reads.erase(it);
		return true;
	}
	return false;
}

void services::COMPLATE_KEY::do_deal_no_order_data()
{
	string str;
	for( ;get_next_data(str) ;)
		recv_cmd(str);
}

void services::COMPLATE_KEY::do_recv_data( PTCP_OVERLAPPDE p_overlapped )
{
	string str;
	if(p_overlapped->n_sequence_number == current_sequnece)
	{
		str.append(p_overlapped->p_buff,p_overlapped->InternalHigh);
		recv_cmd(str);
		++current_sequnece;
		do_deal_no_order_data();
	}else
	{
		push_recv_buf(p_overlapped);
	}
}