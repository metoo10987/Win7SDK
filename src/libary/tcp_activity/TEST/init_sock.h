#pragma once

#include<WinSock2.h>					//initsock.h�ļ�
#pragma comment(lib,"WS2_32")		//���ӵ�WS2_32.LIB

//����ʵ��Winsock���װ����ͷ�
//�ڹ��캯���н��м���Winsock��
//�����������н����ͷ�

//Winsock���װ����ͷŲ�������


//step_1
//����Winsock��ͷ�ļ�#include<WinSock.h>	

//step_2
//���ӵ�WS2_32.LIB��#pragma comment(lib,"WS2_32")

//step_3
//����Winsock��
//	int WSAStartup(
//	  __in   WORD wVersionRequested,	//��Ҫ���ص�Winsock��İ汾�����ֽ�Ϊ�˰汾�ţ����ֽ�Ϊ���汾��
//	  __out  LPWSADATA lpWSAData);		//һ��ָ��WSADATA�ṹ��ָ�룬��������DLL�����ϸ��Ϣ
//ע������ͨ��MAKEWORD(X,Y),��������wVersionRequested��ֵ��X�Ǹ��ֽڣ�Y�ǵ��ֽ�

//step_4
//�ͷ�Winsock��
//WSACleanup()ִ�гɹ�����0



//ע��WSAData�ṹ˵��
//	typedef struct WSAData{
//		WORD			wVersion;								//���ļ�����Ӧ�ó���ʹ�õİ汾
//		WORD			wHightVersion;							//���ļ�֧�ֵ���߰汾
//		char			szDescription[WSADESCRIPTION_LEN+1];	//�������ַ���
//		char			szSystemStatus[WSASYS_STATUS_LEN+1]��	//ϵͳ״̬�ַ���
//		unsigned short	iMaxSockets;							//ͬʱ֧�ֵ�����׽�������
//		unsigned short	iMaxUdpDg;								//2.0�汾���Ѿ������Ĳ���
//		char FAR*		lpVendorInfo;							//2.0�汾���Ѿ������Ĳ���
//	} WSADATA,FAR *LPWSADATA;

class inti_socket
{
public:
	inti_socket(BYTE minorVer = 2,BYTE majorVer = 2);
	~inti_socket(void);
};

