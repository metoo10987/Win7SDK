#ifndef SOCKET_ERROR_H
#define SOCKET_ERROR_H

#include <map>
using namespace std;

namespace socket_error
{
#define MAKE_SOCKET_ERROR_PTR(X) (PBASE_SOCKET_ERROR)(new X())
	inline void handle_socket_error()
	{

	}
	typedef struct BASE_SOCKET_ERROR
	{
		string m_error_info;
		int m_error_code;
		virtual void operator() ()
		{
			handle_socket_error();
		}
	} *PBASE_SOCKET_ERROR;

	typedef struct WSAECONNABORTED_ERROR : public BASE_SOCKET_ERROR
	{	//虚拟链路终止，由于超时或其他故障
		WSAECONNABORTED_ERROR()
		{
			m_error_code = WSAECONNABORTED;
			m_error_info = "由于超时或其他故障,虚拟链路终止";
		}
	} *PWSAECONNABORTED_ERROR;

	typedef struct WSAECONNRESET_ERROR : public BASE_SOCKET_ERROR
	{	//如果是流式套接字，则表示虚拟电路由远程主机重置了，如果是UDP则表之前的操作
		//导致了ICMP“端口不可达”消息
		WSAECONNRESET_ERROR()
		{
			m_error_code = WSAECONNRESET;
			m_error_info = "连接被远程主机重置了或端口不可达";
		}
	} *PWSAECONNRESET;

	typedef struct WSAEFAULT_ERROR : public BASE_SOCKET_ERROR
	{	//lpBuffers, lpNumberOfBytesSent, lpOverlapped, lpCompletionRoutine这些参数有些
		//包含了无效的地址
		WSAEFAULT_ERROR()
		{
			m_error_code = WSAEFAULT;
			m_error_info = "参数中有无效的地址";
		}
	} *PWSAEFAULT_ERROR;

	typedef struct WSAEINVAL_ERROR : public  BASE_SOCKET_ERROR
	{	//套接字尚未绑定或或者没有使用重叠标识创建
		WSAEINVAL_ERROR()
		{
			m_error_code = WSAEINVAL;
			m_error_info = "套接字尚未绑定或或者没有使用重叠标识创建";
		}
	} *PWSAEINTR_ERROR;

	typedef struct WSAENETDOWN_ERROR : public BASE_SOCKET_ERROR
	{
		WSAENETDOWN_ERROR()
		{
			m_error_code = WSAENETDOWN;
			m_error_info = "网络子系统出现了错误";
		}
	} *PWSAENETDOWN_ERROR;

	typedef struct WSAENETRESET_ERROR : public BASE_SOCKET_ERROR
	{
		WSAENETRESET_ERROR()
		{
			m_error_code = WSAENETRESET;
			m_error_info = "由于网络检测操作正在进行，连接被断开";
		}
	} *PWSAENETRESET_ERROR;

	struct WSAENOBUFS_ERROR : public BASE_SOCKET_ERROR
	{
		WSAENOBUFS_ERROR()
		{
			m_error_code = WSAENOBUFS;
			m_error_info = "一个操作在一个套接字能不能进行,因为系统缓冲区不或是因为队列已满";
		}
	};

	struct WSAENOTCONN_ERROR : public BASE_SOCKET_ERROR
	{
		WSAENOTCONN_ERROR()
		{
			m_error_code = WSAENOTCONN;
			m_error_info = "套接字没有连接";
		}
	};

	struct WSAENOTSOCK_ERROR : public BASE_SOCKET_ERROR
	{
		WSAENOTSOCK_ERROR()
		{
			m_error_code = WSAENOTSOCK;
			m_error_info = "不是一个套接字描述符";
		}
	};

	struct WSAESHUTDOWN_ERROR : public BASE_SOCKET_ERROR
	{
		WSAESHUTDOWN_ERROR()
		{
			m_error_code = WSAESHUTDOWN;
			m_error_info = "套接字已经关闭";
		}
	};
	struct WSAEWOULDBLOCK_ERROR : public BASE_SOCKET_ERROR
	{
		WSAEWOULDBLOCK_ERROR()
		{
			m_error_code = WSAEWOULDBLOCK;
			m_error_info = "如果是重叠操作则有太多的重叠请求，不是重叠，但是是非阻塞的操作不会被立即完成，用户可以稍后再次操作";
		}
	};

	struct WSANOTINITIALISED_ERROR : public BASE_SOCKET_ERROR
	{
		WSANOTINITIALISED_ERROR()
		{
			m_error_code = WSANOTINITIALISED;
			m_error_info = "在函数调用前需要成功调用  WSAStartup";
		}
	};

	struct WSA_IO_PENDING_ERROR : public BASE_SOCKET_ERROR
	{
		WSA_IO_PENDING_ERROR()
		{
			m_error_code = WSA_IO_PENDING;
			m_error_info = "重叠操作成功启动,稍后会被标识为完成";
		}
	};

	struct WSA_OPERATION_ABORTED__ERROR : public BASE_SOCKET_ERROR
	{
		WSA_OPERATION_ABORTED__ERROR()
		{
			m_error_code = WSA_OPERATION_ABORTED;
			m_error_info = "重叠操作被取消了";
		}
	};

	struct HANDLE_ERROR
	{
		
		static void init_error_handle_list()
		{
			do_insert(MAKE_SOCKET_ERROR_PTR(WSAECONNABORTED_ERROR));
			do_insert(MAKE_SOCKET_ERROR_PTR(WSAECONNRESET_ERROR));
			do_insert(MAKE_SOCKET_ERROR_PTR(WSAEFAULT_ERROR));
			do_insert(MAKE_SOCKET_ERROR_PTR(WSAEINVAL_ERROR));
			do_insert(MAKE_SOCKET_ERROR_PTR(WSAENETDOWN_ERROR));
			do_insert(MAKE_SOCKET_ERROR_PTR(WSAENETRESET_ERROR));
			do_insert(MAKE_SOCKET_ERROR_PTR(WSAENOBUFS_ERROR));
			do_insert(MAKE_SOCKET_ERROR_PTR(WSAENOTCONN_ERROR));
			do_insert(MAKE_SOCKET_ERROR_PTR(WSAENOTSOCK_ERROR));
			do_insert(MAKE_SOCKET_ERROR_PTR(WSAESHUTDOWN_ERROR));
			do_insert(MAKE_SOCKET_ERROR_PTR(WSAEWOULDBLOCK_ERROR));
			do_insert(MAKE_SOCKET_ERROR_PTR(WSANOTINITIALISED_ERROR));
			do_insert(MAKE_SOCKET_ERROR_PTR(WSA_IO_PENDING_ERROR));
			do_insert(MAKE_SOCKET_ERROR_PTR(WSA_OPERATION_ABORTED__ERROR));
		}

		static void do_handle_error(int error_code)
		{
			map<int,PBASE_SOCKET_ERROR>::iterator it = m_error_handle_list.find(error_code);
			if(it != m_error_handle_list.end())
				return (*(it->second))();
		}
	private:
		static void do_insert(PBASE_SOCKET_ERROR p_socket_error)
		{
			m_error_handle_list.insert(map<int,PBASE_SOCKET_ERROR>::value_type(
				p_socket_error->m_error_code,p_socket_error));
		}

		static map<int,PBASE_SOCKET_ERROR> m_error_handle_list;
	};
}


#endif	//SOCKET_ERROR_H