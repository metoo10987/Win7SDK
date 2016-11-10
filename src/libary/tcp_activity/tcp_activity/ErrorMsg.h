//错误消息类用于将错误ID转换成错误描述文本，转换过程通过
//FormateMessage函数完成

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

