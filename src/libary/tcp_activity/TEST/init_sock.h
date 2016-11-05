#pragma once

#include<WinSock2.h>					//initsock.h文件
#pragma comment(lib,"WS2_32")		//链接到WS2_32.LIB

//该类实现Winsock库的装入和释放
//在构造函数中进行加载Winsock库
//在析构函数中进行释放

//Winsock库的装入和释放步骤如下


//step_1
//包含Winsock库头文件#include<WinSock.h>	

//step_2
//链接到WS2_32.LIB库#pragma comment(lib,"WS2_32")

//step_3
//加载Winsock库
//	int WSAStartup(
//	  __in   WORD wVersionRequested,	//想要加载的Winsock库的版本，高字节为此版本号，低字节为主版本号
//	  __out  LPWSADATA lpWSAData);		//一个指向WSADATA结构的指针，用来返回DLL库的详细信息
//注：可以通过MAKEWORD(X,Y),宏来建立wVersionRequested的值，X是高字节，Y是低字节

//step_4
//释放Winsock库
//WSACleanup()执行成功返回0



//注：WSAData结构说明
//	typedef struct WSAData{
//		WORD			wVersion;								//库文件建议应用程序使用的版本
//		WORD			wHightVersion;							//库文件支持的最高版本
//		char			szDescription[WSADESCRIPTION_LEN+1];	//库描述字符串
//		char			szSystemStatus[WSASYS_STATUS_LEN+1]；	//系统状态字符串
//		unsigned short	iMaxSockets;							//同时支持的最大套接字数量
//		unsigned short	iMaxUdpDg;								//2.0版本中已经废弃的参数
//		char FAR*		lpVendorInfo;							//2.0版本中已经废弃的参数
//	} WSADATA,FAR *LPWSADATA;

class inti_socket
{
public:
	inti_socket(BYTE minorVer = 2,BYTE majorVer = 2);
	~inti_socket(void);
};

