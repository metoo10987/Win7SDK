#include "init_sock.h"


inti_socket::inti_socket(BYTE minorVer,BYTE majorVer)
{
	//��ʼ��WS2_32.LIB
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(minorVer,majorVer);//minorVer���ֽڴΰ汾�� ��majorVer���ֽ����汾��
	if(WSAStartup(sockVersion,&wsaData) !=0 )
	{
		//WSAStartup����Winsock�⣬�ɹ�����0
		exit(0);
	}
}


inti_socket::~inti_socket(void)
{
	WSACleanup();
}
