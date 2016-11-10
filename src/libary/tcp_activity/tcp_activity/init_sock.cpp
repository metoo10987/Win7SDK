#include "init_sock.h"


inti_socket::inti_socket(BYTE minorVer,BYTE majorVer)
{
	//初始化WS2_32.LIB
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(minorVer,majorVer);//minorVer高字节次版本号 ，majorVer低字节主版本号
	if(WSAStartup(sockVersion,&wsaData) !=0 )
	{
		//WSAStartup加载Winsock库，成功返回0
		exit(0);
	}
}


inti_socket::~inti_socket(void)
{
	WSACleanup();
}
