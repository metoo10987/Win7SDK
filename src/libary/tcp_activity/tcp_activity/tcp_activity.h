#ifndef TCP_ACTIVITY_H
#define TCP_ACTIVITY_H

static int ovelapped_count =0;
static int complate_key_count = 0;
static int read_count = 0;
static int recv_bytes = 0;
//相关函数说明：
// 
/*	WSAAccept
	SOCKET WSAAccept(
		_In_     SOCKET s,											//用于监听的套接字
		_Out_    struct sockaddr *addr,						//收到连接后返回客户端的地址
		_Inout_  LPINT addrlen,									//客户端地址长度
		_In_     LPCONDITIONPROC lpfnCondition,	//函数指针，指向一个函数，这个函数用于通过条件判断是接受还是拒绝，依据的参数是dwCallbackData，不用的话可以为NULL
		_In_     DWORD_PTR dwCallbackData			//传递给lpfnCondition的参数如果lpfnCondition为NULL则这个函数也为NULL
	);
	如果没有错误发生该函数返回客户端套接字句柄，否则返回INVALID_SOCKET
*/
#define TCP_BUFFER_MAX_SIZE 1024*8
#define DATA_BUFSIZE 1024*8


#include "init_sock.h"
#include "ErrorMsg.h"
#include "io_complate_port.h"
#include "socket_error.h"
#include "string_separater.h"
#include "net_packet.h"
#include "network_file_transfer.h"
using namespace socket_error;

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <functional>
#include <MSWSock.h>

using namespace std;


namespace services
{
#define		OP_READ	1
#define		OP_WRITE	2
#define		OP_ACCEPT 3
#define		OP_CONNECT 4
	class base_tcp_stream
	{
	public:
		base_tcp_stream(){}
		base_tcp_stream(unsigned long key);
		virtual ~base_tcp_stream();
		virtual void do_send(const string& str);
		typedef std::tr1::function<void (unsigned long,const char*,int)> cmd_send_type;
		cmd_send_type m_post_send_cdm;
		unsigned long m_key;
		CRITICAL_SECTION send_cs;
	};

	class base_client_stream : public base_tcp_stream
	{
	public:
		base_client_stream();
		base_client_stream(unsigned long key);
		virtual ~base_client_stream();

		virtual void handle_read(string recv_buf);
		virtual void handle_read_fail(int error_code);
		virtual void send(net_packet* p_send_packet);
		void set_socket_addr(SOCKET s);
		string get_remote_ip();
		int get_remote_port();
		SOCKADDR_IN   socket_addr;
		string m_recvbuf;
	};

	template<typename T = base_client_stream>
	class base_server_stream : public base_tcp_stream
	{
	public:
		base_server_stream(){}
		virtual ~base_server_stream(){}
		typedef T client_stream_type;
		virtual void handle_accept(client_stream_type* p_client_stream)
		{}

		virtual void handle_connected(client_stream_type* p_client_stream)
		{	// 客户端连接成功自动调用此函数
		}
		virtual client_stream_type* create_client_stream()
		{
			return new client_stream_type();
		}
	};

	// 定义OVERLAPPED的结构体
	typedef struct TCP_OVERLAPPED : public OVERLAPPED
	{
		TCP_OVERLAPPED();
		~TCP_OVERLAPPED();

		char* p_buff;											//	I/O操作使用的缓冲区
		int n_len;												//缓冲区使用的大小
		int n_operation_type;							//操作类型
		unsigned long n_sequence_number;		//此I/O的序列号
		SOCKET s;
	} *PTCP_OVERLAPPDE;

	//	定义complate key结构体
	typedef struct COMPLATE_KEY
	{
		int n;
		COMPLATE_KEY(SOCKET s);
		COMPLATE_KEY();
		~COMPLATE_KEY();
		void init_members();
		//锁操作
		void init_cs();
		void en_lock();
		void un_lock();
		//接收数据
		void handle_read(PTCP_OVERLAPPDE p_overlapped);
		void do_recv_data(PTCP_OVERLAPPDE p_overlapped);
		void push_recv_buf(PTCP_OVERLAPPDE p_overlapped);
		bool get_next_data(string& str);
		void do_deal_no_order_data();
		//关闭套接字，释放资源
		void close();
		void release_rec();
		//返回没有完成的 IO 数量
		int get_pending_io_count();

		SOCKET s;											//套接字句柄
		SOCKADDR_IN remote_addr;				//连接的远程地址
		SOCKADDR_IN local_addr;					//连接的本地地址
		bool b_closing;									//套接字是否关闭
		int n_out_standing_recv_count;			//此套接字上抛出的recv未决重叠操作数量
		int n_out_standing_send_count;		//此套接字上抛出的send未决重叠操作数量
		unsigned long read_sequence;			//安排给下一个接收的序列号
		unsigned long current_sequnece;		//当前要读的序列号
		CRITICAL_SECTION lock;					
		map<unsigned long,string> no_order_reads;	//没有按顺序完成的读I/O

		typedef std::tr1::function<void (string buffer)> recv_type;
		typedef std::tr1::function<void(int)> cmd_delete_connect;
		cmd_delete_connect delete_connect;
		cmd_delete_connect handle_read_fail;
		cmd_delete_connect handle_write_fail;
		cmd_delete_connect handle_accept_fail;
		recv_type recv_cmd;
	} *PCOMPLATE_KEY;

	struct work_item
	{
		typedef std::tr1::function<void()> cmd_handle_io_type;
		cmd_handle_io_type handle_io;
		void do_handle()
		{
			while(1)
			{
				handle_io();
			}
		}
	};
	
	inline DWORD WINAPI thread_pool_callback(LPVOID pvContext)
	{
		work_item* p = (work_item*)pvContext;
		p->do_handle();
		return 0;
	}

	inline void close_complate_key(PCOMPLATE_KEY& p_key)
	{
		p_key->close();
		if(p_key->get_pending_io_count() == 0)
		{
			p_key->handle_read_fail(1);
			p_key->release_rec();
			p_key = NULL;
		}
	}

	template< typename T1 = base_client_stream, typename T2 = base_server_stream<T1> >
	class tcp_activity
	{
	public:
		tcp_activity():m_io_complate_port(6)
		{
			InitializeCriticalSection(&m_cs);
			m_pserver_stream = new T2();
			m_handle_io_item.handle_io = std::tr1::bind(&tcp_activity<T1,T2>::handle_io,this);
			init_handle_io_thread(4);
		}
		~tcp_activity()
		{
			DeleteCriticalSection(&m_cs);
		}

		void start_listen(int port, int max_wait_num = 10)
		{
			init_socket();
			init_socket_addr(m_si,port);
			do_listen(max_wait_num);
		}

		void to_connect(const string& ip, int port)
		{
			do_connect(ip,port);
		}

		void handle_io()
		{
			DWORD translate_num;
			PCOMPLATE_KEY p_key = NULL;
			PTCP_OVERLAPPDE p_overlapped = NULL;

			m_io_complate_port.wait_complate_status(&translate_num,
				(PULONG_PTR)&p_key,(LPOVERLAPPED*)&p_overlapped,INFINITE);

			switch(p_overlapped->n_operation_type)
			{
			case OP_READ:
				p_key->en_lock();
				--p_key->n_out_standing_recv_count;
				if(translate_num == 0)//对方关闭了套接字
					close_complate_key(p_key);
				else
				{
					p_key->handle_read(p_overlapped);
					int error_code = post_recv(p_key);
					if(error_code != 0 && error_code != WSA_IO_PENDING)
						close_complate_key(p_key);
				}
				if(p_key != NULL)
					p_key->un_lock();
				break;
			case OP_WRITE:
				p_key->en_lock();
				--p_key->n_out_standing_send_count;
				p_key->un_lock();
				break;
			case OP_ACCEPT:
				do_accept(p_overlapped->s);
				post_accept();
				break;
			case OP_CONNECT:
				handle_connect(p_key);
				break;
			}
			delete p_overlapped;
		}
	private:
		io_complate_port m_io_complate_port;
		SOCKET m_socket;
		SOCKADDR_IN m_si;
		vector<SOCKET> m_client_socket_list;
		inti_socket m_init_socket;
		map<unsigned long,PCOMPLATE_KEY> m_key_list;
		typedef T1 client_stream_type;
		typedef T2 server_stream_type;
		server_stream_type* m_pserver_stream;
		work_item m_handle_io_item;
		LPFN_ACCEPTEX m_lpfnAcceptEx;	// AcceptEx函数地址
		LPFN_CONNECTEX m_lpfn_connect_ex;// ConnectEx函数地址 
		CRITICAL_SECTION m_cs;

		void en_lock()
		{
			EnterCriticalSection(&m_cs);
		}
		void un_lock()
		{
			LeaveCriticalSection(&m_cs);
		}

		void insert_connect(PCOMPLATE_KEY p_complate_key)
		{
			en_lock();
			m_key_list.insert(map<unsigned long, PCOMPLATE_KEY>::value_type(
				(unsigned long)(p_complate_key),p_complate_key));
			un_lock();
		}

		void bind_stream_handler(PCOMPLATE_KEY p_complate_key, client_stream_type* p_client_stream)
		{
			p_complate_key->delete_connect = std::tr1::bind(
				&tcp_activity<T1,T2>::delete_connect,this,std::tr1::placeholders::_1);
			p_complate_key->handle_read_fail = std::tr1::bind(
				&client_stream_type::handle_read_fail,p_client_stream,std::tr1::placeholders::_1);
			p_complate_key->recv_cmd = std::tr1::bind(
				&client_stream_type::handle_read,p_client_stream,std::tr1::placeholders::_1);
			p_client_stream->m_post_send_cdm = std::tr1::bind(&tcp_activity<T1,T2>::do_send,this,
				std::tr1::placeholders::_1,std::tr1::placeholders::_2,std::tr1::placeholders::_3);
		}

		void delete_connect(int key)
		{	//删除一个连接
			en_lock();
			map<unsigned long,PCOMPLATE_KEY>::iterator it = m_key_list.find(key);
			if(it != m_key_list.end())
			{
				PCOMPLATE_KEY p = it->second;
				m_key_list.erase(key);
				delete p;
			}
			un_lock();
		}

		
		void init_handle_io_thread(int thread_count)
		{	// 初始化I/O处理线程
			for(int i = 0; i < thread_count; ++i)
				QueueUserWorkItem(
				thread_pool_callback,
				(LPVOID)&m_handle_io_item,
				WT_EXECUTELONGFUNCTION);
		}
		
		void init_socket()
		{	//初始化套接字信息接口
			m_socket = ::WSASocket(AF_INET, SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
			if(m_socket == INVALID_SOCKET)
				cout << "创建套接字失败"<< endl;
			return;
		}

		bool initlize_socket(SOCKET& s)
		{
			s = ::WSASocket(AF_INET, SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
			if(s == INVALID_SOCKET)
				return false;
			return true;
		}
		void init_socket_addr(SOCKADDR_IN& sock_addr, int nport)
		{
			sock_addr.sin_family = AF_INET;
			sock_addr.sin_port = ::htons(nport);
			sock_addr.sin_addr.S_un.S_addr = INADDR_ANY;
		}

		void init_socket_addr(SOCKADDR_IN& sock_addr, const string& ip, int nport)
		{
			init_socket_addr(sock_addr, nport);
			sock_addr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
		}

		//开启监听与连接接口
		void do_listen(int max_wait_num)
		{	
			if(::bind(m_socket, (sockaddr*)&m_si, sizeof(m_si) ) == SOCKET_ERROR)
				cout << "绑定错误" << endl;
			if(::listen(m_socket, max_wait_num) == SOCKET_ERROR)
				cout <<"监听错误"<<endl;
			// 加载扩展函数AcceptEx
			GUID GuidAcceptEx = WSAID_ACCEPTEX;
			DWORD dwBytes;
			::WSAIoctl(m_socket, 
				SIO_GET_EXTENSION_FUNCTION_POINTER, 
				&GuidAcceptEx, 
				sizeof(GuidAcceptEx),
				&m_lpfnAcceptEx, 
				sizeof(m_lpfnAcceptEx), 
				&dwBytes, 
				NULL, 
				NULL);
			PCOMPLATE_KEY p_complate_key = new COMPLATE_KEY(m_socket);
			m_io_complate_port.associate_to_device((HANDLE)m_socket,(ULONG)p_complate_key);
			post_accept();
		}

		bool handle_connect(PCOMPLATE_KEY p_complate_key)
		{
			//更新SOCKET状态
			setsockopt( p_complate_key->s, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, NULL, 0 );
			int seconds;
			int bytes = sizeof(seconds);
			int iResult = 0;
			wstring str;
			iResult = getsockopt( p_complate_key->s, SOL_SOCKET, SO_CONNECT_TIME,
				(char *)&seconds, (PINT)&bytes );
			if ( iResult != NO_ERROR ) {
				printf( "getsockopt(SO_CONNECT_TIME) failed with error: %u\n", 
					WSAGetLastError() );
			}
			else {
				if (seconds == 0xFFFFFFFF)
				{
					printf("Connection not established yet\n");
				}
				else
				{
					insert_connect(p_complate_key);
					client_stream_type* p_client_stream = new client_stream_type((unsigned long)(p_complate_key));
					p_client_stream->set_socket_addr(p_complate_key->s);
					bind_stream_handler(p_complate_key,p_client_stream);
					m_pserver_stream->handle_connected(p_client_stream);
					post_recv(p_complate_key);
				}
			}
			return true;
		}

		void do_connect(const string& ip, int nport)
		{
			PCOMPLATE_KEY p_complate_key = new COMPLATE_KEY();
			if(initlize_socket(p_complate_key->s))
			{
				GUID GuidConnectEx = WSAID_CONNECTEX;
				DWORD dwBytes;
				int code = ::WSAIoctl(p_complate_key->s, 
					SIO_GET_EXTENSION_FUNCTION_POINTER, 
					&GuidConnectEx, 
					sizeof(GuidConnectEx),
					&m_lpfn_connect_ex, 
					sizeof(m_lpfn_connect_ex), 
					&dwBytes, 
					NULL, 
					NULL);
				m_io_complate_port.associate_to_device((HANDLE)p_complate_key->s,(ULONG_PTR)p_complate_key);
				post_connect(p_complate_key,ip,nport);
			}
		}

		int post_connect(PCOMPLATE_KEY p_complate_key,const string& ip,int port)
		{	
			sockaddr_in sin;
			sin.sin_family = AF_INET;
			sin.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
			sin.sin_port = htons(port);

			SOCKADDR_IN LocalIP = {};  
			LocalIP.sin_family = AF_INET;  
			LocalIP.sin_addr.s_addr = INADDR_ANY;  
			LocalIP.sin_port = htons( (short)0 );    //使用0让系统自动分配  

			if(::bind(p_complate_key->s, (sockaddr*)&LocalIP, sizeof(LocalIP) ) == SOCKET_ERROR)
				cout << "绑定错误"<<endl;
			PTCP_OVERLAPPDE p_overlapped = new TCP_OVERLAPPED();
			p_overlapped->n_operation_type = OP_CONNECT;
			DWORD send_bytes;
			DWORD send_data_langth = 0;
			BOOL b = m_lpfn_connect_ex(p_complate_key->s, 
				(SOCKADDR *)&sin,
				sizeof(sin), 
				p_overlapped->p_buff,
				send_data_langth, 
				&send_bytes, 
				p_overlapped);
			if(!b)
			{
				wstring error = CErrorMsg::getMsg();
				return GetLastError();
			}
			return 0;
		}
		void do_accept(SOCKET s)
		{
			PCOMPLATE_KEY p_complate_key= new COMPLATE_KEY(s);
			
			if(!m_io_complate_port.associate_to_device((HANDLE)p_complate_key->s,(ULONG)p_complate_key))
				cout << "与 IO端口关联失败"<<endl;
			insert_connect(p_complate_key);

			client_stream_type* p_client_stream = new client_stream_type((unsigned long)p_complate_key);
			bind_stream_handler(p_complate_key,p_client_stream);
			m_pserver_stream->handle_accept(p_client_stream);
			//投递接收I/O
			int error_code = post_recv(p_complate_key);
			if(error_code !=0 && error_code!= WSA_IO_PENDING)
				p_complate_key->close();
		}

		//投递 I/O错误处理
		void post_io_error(PCOMPLATE_KEY p_complate_key, PTCP_OVERLAPPDE p_overlapped)
		{
			int error_code = WSAGetLastError();
			switch()
		}
		//投递 I/O请求接口

		int post_accept()
		{
			SOCKET client = ::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
			DWORD dwBytes;
			PTCP_OVERLAPPDE p_overlapped = new TCP_OVERLAPPED();
			p_overlapped->s = client;
			p_overlapped->n_operation_type = OP_ACCEPT;
			p_overlapped->n_len = 1;
			BOOL b = m_lpfnAcceptEx(m_socket, 
				p_overlapped->s,
				p_overlapped->p_buff, 
				0,
				sizeof(sockaddr_in) + 16, 
				sizeof(sockaddr_in) + 16, 
				&dwBytes, 
				p_overlapped);
			if(!b)
			{
				wstring error = CErrorMsg::getMsg();
				return GetLastError();
			}
			return 0;
		}
		int post_recv(PCOMPLATE_KEY p_complate_key)
		{

			WSABUF data_buf;

			DWORD translate_num = 0;
			DWORD flag = 0;
			PTCP_OVERLAPPDE p_overlapped = new TCP_OVERLAPPED();
			data_buf.buf = p_overlapped->p_buff;
			data_buf.len = p_overlapped->n_len;
			p_overlapped->n_operation_type = OP_READ;

			p_complate_key->en_lock();
			++p_complate_key->n_out_standing_recv_count;
			p_overlapped->n_sequence_number = p_complate_key->read_sequence;
			p_complate_key->read_sequence = p_complate_key->read_sequence+1;
			int error = WSARecv(p_complate_key->s,&data_buf,
				1,
				&translate_num,
				&flag,
				p_overlapped,NULL);
			if(error !=0)
			{
				error = GetLastError();
				if(error != WSA_IO_PENDING)
				{	//投递出错
					--p_complate_key->n_out_standing_recv_count;
					delete p_overlapped;
				}
			}
			p_complate_key->un_lock();
			return error;
		}

		void do_send(unsigned long key,const char* p_buf, int len)
		{
			en_lock();
			map<unsigned long,PCOMPLATE_KEY>::iterator it = m_key_list.find(key);
			PCOMPLATE_KEY p_complate_key = NULL;
			if(it != m_key_list.end())
				p_complate_key = it->second;
			un_lock();
			if(p_complate_key != NULL)
				post_send(p_complate_key,p_buf,len);
		}

		int post_send(PCOMPLATE_KEY p_complate_key, const char* p_buf, int len)
		{
			WSABUF buf;
			PTCP_OVERLAPPDE p_overlapped = new TCP_OVERLAPPED();
			memcpy(p_overlapped->p_buff,p_buf,len);
			p_overlapped->n_len = len;
			p_overlapped->n_operation_type = OP_WRITE;
			DWORD translate_bytes = 0;
			buf.buf = p_overlapped->p_buff;
			buf.len = p_overlapped->n_len;
			p_complate_key->en_lock();
			++p_complate_key->n_out_standing_send_count;
			int error_code = WSASend(p_complate_key->s,&buf,
				1,
				&translate_bytes,
				0,
				p_overlapped,NULL);
			if(error_code !=0)
			{
				error_code = GetLastError();
				if(error_code != WSA_IO_PENDING)
				{
					--p_complate_key->n_out_standing_send_count;
					delete p_overlapped;
				}
			}
			p_complate_key->un_lock();
			return error_code;
		}

		void post_request(SOCKET s,int request_type)
		{
			WSABUF buf;
			DWORD dwflag = 0;
			PTCP_OVERLAPPDE p_overlapped = new _TCP_OVERLAPPED();
			switch(request_type)
			{
			case 0:
				::WSARecv(s,&buf,1,0, &dwflag,(LPOVERLAPPED)p_overlapped,NULL);
				return;
			case 1:
				::WSARecv(s,&buf,1,0,&dwflag,(LPOVERLAPPED)p_overlapped,NULL);
				return;
			}
		}
	};
}
#endif	//	TCP_ACTIVITY_H