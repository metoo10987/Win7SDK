//////////////////////////////////////////////////////////////////////////
//	ÍøÂçÎÄ¼ş´«Êä
#include "tcp_activity.h"

#include <fstream>
template <typename T = services::base_client_stream>
class network_file_transfer
{
public:
	typedef T client_stream;
	network_file_transfer( client_stream* p_client_stream, string filename ,bool open_flag) : 
	m_p_client_stream(m_p_client_stream) , m_filename(filename)
	{
		if(open_flag == true)
			m_file_stream.open(filename.c_str(),ios::in|ios::binary);
		else
			m_file_stream.open(filename.c_str(),ios::out | ios::binary);
	}
	network_file_transfer(){}
	void init_transfer(client_stream* p_client_stream, string filename ,bool open_flag)
	{
		m_p_client_stream = p_client_stream;
		m_filename = filename;
		if(open_flag == true)
			m_file_stream.open(filename.c_str(),ios::in|ios::binary);
		else
			m_file_stream.open(filename.c_str(),ios::out | ios::binary);
	}
	~network_file_transfer()
	{
		m_file_stream.close();
	}
	static DWORD WINAPI send_thread( LPVOID lpParameter)
	{
		network_file_transfer* p_tranfer = (network_file_transfer*)lpParameter;
		p_tranfer->do_send();
		//delete p_tranfer;
		return 0;
	}

	void do_send()
	{
		if(m_file_stream.is_open())
		{
			istreambuf_iterator<char> beg(m_file_stream),end;
			string str_(beg,end);
			m_file_stream>>str_;
			int i = 0;
			for( ; str_.size() > 1024*7; )
			{
				net_packet packet;
				packet.en_code("N_F_T_",i++,"0000000000",str_.c_str(),1024*7);
				m_p_client_stream->send(&packet);
				str_.erase(str_.begin(),str_.begin()+1024*7);
				Sleep(10);
			}
			if(str_.size() > 0)
			{
				net_packet packet;
				packet.en_code("N_F_T_",i++,"0000000000",str_.c_str(),str_.size());
				m_p_client_stream->send(&packet);
			}
			m_file_stream.close();
		}
	}
public:
	inline void start_send()
	{
		CreateThread(NULL,0,&network_file_transfer<>::send_thread,(LPVOID)this,0,NULL);
	}
	inline void write_file(string str)
	{
		if(m_file_stream.is_open())
		{
			m_file_stream.write(str.c_str(),str.size());
			m_file_stream.flush();
		}
	}
private:
	client_stream* m_p_client_stream;
	string m_filename;
	fstream m_file_stream;
};

