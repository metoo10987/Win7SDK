//	网络客户端持有器，持有 client_net_stream实例,和对应的重发缓冲区

#pragma once
#include "client_net_stream.h"

#include <queue>
class client_net_stream_holder
{
public:
	typedef map<string,client_net_stream*> stream_list_type;
	typedef map<string,deque<net_packet*>> resend_buffer_list_type;
	typedef map<string,string> recv_buffer_list_type;

#define MAKE_STREAM(x,y) stream_list_type::value_type(x,y)
#define INSERT_STREAM(x,y) m_stream_list.insert(MAKE_STREAM(x,y))

	client_net_stream_holder(void);
	~client_net_stream_holder(void);

	void init_stream(string key, client_net_stream* p_stream)
	{
		INSERT_STREAM(key, p_stream);
	}

	void send(string& stream_key, string& mark, string& key, string& content)
	{
		stream_list_type::iterator it = m_stream_list.find(key);
		if( it != m_stream_list.end() )
		{
			net_packet packet;
			send(it->second, &packet);
		}
	}

	void send(client_net_stream* p_stream, net_packet* p_net_packet)
	{
		p_stream->send(p_net_packet);
	}
	void add_send(string& stream_key, string& mark, string& key, string& content)
	{
		resend_buffer_list_type::iterator resend_it = m_stream_resend_buffer_list.find(stream_key);
		stream_list_type::iterator stream_it = m_stream_list.find(key);
		if( stream_it != m_stream_list.end() )
		{
			net_packet* p_net_packet = new net_packet();
			p_net_packet->en_code(mark.c_str(),-1,key.c_str(),content.c_str(), content.size());
			send(stream_it->second, p_net_packet);
			if( resend_it != m_stream_resend_buffer_list.end())
				resend_it->second.push_back(p_net_packet);
		}
	}
private:
	stream_list_type m_stream_list;
	resend_buffer_list_type m_stream_resend_buffer_list;
	recv_buffer_list_type m_stream_recv_buffer_list;
};

