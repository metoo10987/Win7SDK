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
	{	//������·��ֹ�����ڳ�ʱ����������
		WSAECONNABORTED_ERROR()
		{
			m_error_code = WSAECONNABORTED;
			m_error_info = "���ڳ�ʱ����������,������·��ֹ";
		}
	} *PWSAECONNABORTED_ERROR;

	typedef struct WSAECONNRESET_ERROR : public BASE_SOCKET_ERROR
	{	//�������ʽ�׽��֣����ʾ�����·��Զ�����������ˣ������UDP���֮ǰ�Ĳ���
		//������ICMP���˿ڲ��ɴ��Ϣ
		WSAECONNRESET_ERROR()
		{
			m_error_code = WSAECONNRESET;
			m_error_info = "���ӱ�Զ�����������˻�˿ڲ��ɴ�";
		}
	} *PWSAECONNRESET;

	typedef struct WSAEFAULT_ERROR : public BASE_SOCKET_ERROR
	{	//lpBuffers, lpNumberOfBytesSent, lpOverlapped, lpCompletionRoutine��Щ������Щ
		//��������Ч�ĵ�ַ
		WSAEFAULT_ERROR()
		{
			m_error_code = WSAEFAULT;
			m_error_info = "����������Ч�ĵ�ַ";
		}
	} *PWSAEFAULT_ERROR;

	typedef struct WSAEINVAL_ERROR : public  BASE_SOCKET_ERROR
	{	//�׽�����δ�󶨻����û��ʹ���ص���ʶ����
		WSAEINVAL_ERROR()
		{
			m_error_code = WSAEINVAL;
			m_error_info = "�׽�����δ�󶨻����û��ʹ���ص���ʶ����";
		}
	} *PWSAEINTR_ERROR;

	typedef struct WSAENETDOWN_ERROR : public BASE_SOCKET_ERROR
	{
		WSAENETDOWN_ERROR()
		{
			m_error_code = WSAENETDOWN;
			m_error_info = "������ϵͳ�����˴���";
		}
	} *PWSAENETDOWN_ERROR;

	typedef struct WSAENETRESET_ERROR : public BASE_SOCKET_ERROR
	{
		WSAENETRESET_ERROR()
		{
			m_error_code = WSAENETRESET;
			m_error_info = "����������������ڽ��У����ӱ��Ͽ�";
		}
	} *PWSAENETRESET_ERROR;

	struct WSAENOBUFS_ERROR : public BASE_SOCKET_ERROR
	{
		WSAENOBUFS_ERROR()
		{
			m_error_code = WSAENOBUFS;
			m_error_info = "һ��������һ���׽����ܲ��ܽ���,��Ϊϵͳ��������������Ϊ��������";
		}
	};

	struct WSAENOTCONN_ERROR : public BASE_SOCKET_ERROR
	{
		WSAENOTCONN_ERROR()
		{
			m_error_code = WSAENOTCONN;
			m_error_info = "�׽���û������";
		}
	};

	struct WSAENOTSOCK_ERROR : public BASE_SOCKET_ERROR
	{
		WSAENOTSOCK_ERROR()
		{
			m_error_code = WSAENOTSOCK;
			m_error_info = "����һ���׽���������";
		}
	};

	struct WSAESHUTDOWN_ERROR : public BASE_SOCKET_ERROR
	{
		WSAESHUTDOWN_ERROR()
		{
			m_error_code = WSAESHUTDOWN;
			m_error_info = "�׽����Ѿ��ر�";
		}
	};
	struct WSAEWOULDBLOCK_ERROR : public BASE_SOCKET_ERROR
	{
		WSAEWOULDBLOCK_ERROR()
		{
			m_error_code = WSAEWOULDBLOCK;
			m_error_info = "������ص���������̫����ص����󣬲����ص��������Ƿ������Ĳ������ᱻ������ɣ��û������Ժ��ٴβ���";
		}
	};

	struct WSANOTINITIALISED_ERROR : public BASE_SOCKET_ERROR
	{
		WSANOTINITIALISED_ERROR()
		{
			m_error_code = WSANOTINITIALISED;
			m_error_info = "�ں�������ǰ��Ҫ�ɹ�����  WSAStartup";
		}
	};

	struct WSA_IO_PENDING_ERROR : public BASE_SOCKET_ERROR
	{
		WSA_IO_PENDING_ERROR()
		{
			m_error_code = WSA_IO_PENDING;
			m_error_info = "�ص������ɹ�����,�Ժ�ᱻ��ʶΪ���";
		}
	};

	struct WSA_OPERATION_ABORTED__ERROR : public BASE_SOCKET_ERROR
	{
		WSA_OPERATION_ABORTED__ERROR()
		{
			m_error_code = WSA_OPERATION_ABORTED;
			m_error_info = "�ص�������ȡ����";
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