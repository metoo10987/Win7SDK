//������Ϣ�����ڽ�����IDת���ɴ��������ı���ת������ͨ��
//FormateMessage�������

#pragma once
#include <string>
#include <Windows.h>
using namespace std;
class CErrorMsg
{
public:
	CErrorMsg(void);
	~CErrorMsg(void);
	static wstring getMsg(int error_code);
	static wstring getMsg();
};

