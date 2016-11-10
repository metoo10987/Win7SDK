#include "client_net_stream.h"


client_net_stream::client_net_stream(void)
{
}

client_net_stream::client_net_stream(unsigned long key) : base_client_stream(key)
{
}

client_net_stream::~client_net_stream(void)
{
}

void client_net_stream::handle_read( string recv_buf )
{
	m_recvbuf += recv_buf;
	net_packet packet;
	int dealdata_length = 0;
	dealdata_length = packet.de_code(m_recvbuf.c_str(),m_recvbuf.size());
	if(dealdata_length == 0)
		return;
	for( ;dealdata_length > 0; )
	{
		deal_cmd(packet);
		m_recvbuf.erase(m_recvbuf.begin(),m_recvbuf.begin()+dealdata_length);
		dealdata_length = packet.de_code(m_recvbuf.c_str(),m_recvbuf.size());
	}
}

void client_net_stream::handle_read_fail( int error_code )
{
}

void client_net_stream::to_send( string& mark, string& key, string& str )
{
	net_packet packet;
	packet.en_code(mark.c_str(), -1, key.c_str(), str.c_str(), str.size() );
	send(&packet);
}

void client_net_stream::register_cmd( string key, cmd_type cmd )
{
	m_cmd_list.insert(MAKE_VALUE(key, cmd));
}

void client_net_stream::delete_all_cmd()
{
	m_cmd_list.clear();
}

void client_net_stream::delete_a_cmd( string key )
{
	m_cmd_list.erase(key);
}

void client_net_stream::deal_cmd( net_packet& packet )
{
	string key;
	string content;
	key.append(packet.c_mark,MARK_LENGTH);

	char* p_content = NULL;
	int content_length = 0;
	p_content = packet.data(content_length);
	content.append(p_content, content_length);

	cmd_list_type::iterator it = m_cmd_list.find(key);
	if(it != m_cmd_list.end())
		it->second(p_content);
}
