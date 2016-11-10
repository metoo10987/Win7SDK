#ifndef IPC_INCLUDE
#define IPC_INCLUDE

#include <functional>
#include <string>
using namespace std;
#ifdef IPC_API
#else
#define IPC_API extern "C" __declspec(dllimport)
#endif	//	IPC_API

typedef std::tr1::function<void(char* , int)> handle_type;

// init_ipc ��ʼ�� IPC Ӧ���ȵ��øú���
IPC_API void init_ipc();

// ����һ��������������name�ǻ�������
// ������to_send������������ֵĻ�������������ʱ�����Զ����յ�����
// ������ͨ��register_read_handlerע��Ĵ�����
IPC_API void start_listen(string name);

//	���ӵ�һ��������nameҪ���ӵ��Ļ����������֣�
//	���Ӻ����ͨ��to_send�����������д����
IPC_API void to_connect(string name);

//	��һ����Ϊname�Ļ������������ݣ��ڵ��øú���ǰ��������������Ѿ�ͨ��to_connect������
//	p_strָ��һ��������������������е����ݽ���д�뵽name�����Ļ�������
//	langthд��name�����������ݵĳ���
IPC_API void to_send(string name, const char* p_src, int langth);

//	ע��һ�����ݴ����������������Ļ�����������д��ʱ�����Զ����ñ�ע��ĺ���
IPC_API void register_read_handler(handle_type handler);

#endif	// IPC_INCLUDE

