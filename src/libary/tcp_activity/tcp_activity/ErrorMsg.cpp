//#include "StdAfx.h"
#include "ErrorMsg.h"

CErrorMsg::CErrorMsg(void)
{
}

CErrorMsg::~CErrorMsg(void)
{
}

std::wstring CErrorMsg::getMsg( int error_code )
{
	HLOCAL hlocal = NULL;
	//生成语音标识
	//用LANG_NEUTRAL和SUBLANG_NEUTRAL生成操作系统默认语言
	DWORD systemLocale = MAKELANGID(LANG_NEUTRAL,SUBLANG_NEUTRAL);
	BOOL fok = FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM |//取得系统定义的错误代码对应的字符串
		FORMAT_MESSAGE_IGNORE_INSERTS |//允许获得含有%站位符的消息，这些占位符用来提供更多上下文相关信息
		FORMAT_MESSAGE_ALLOCATE_BUFFER,//要求函数分配一个足以容纳错误文本描述的内存，这块内存的句柄将在hlocal中返回
		NULL,
		error_code,
		systemLocale,
		(PTSTR)&hlocal,
		0,
		NULL
		);
	if(!fok){
		//is it a network related error?
		HMODULE hmodule = LoadLibraryEx(TEXT("netmsg.dll"),NULL,
			DONT_RESOLVE_DLL_REFERENCES);
		if(hmodule != NULL){
			fok = FormatMessage(
				FORMAT_MESSAGE_FROM_HMODULE |
				FORMAT_MESSAGE_IGNORE_INSERTS |
				FORMAT_MESSAGE_ALLOCATE_BUFFER,
				hmodule,
				error_code,
				systemLocale,
				(PTSTR)&hlocal,
				0,
				NULL
				);
		}
	}
	if(fok && (hlocal != NULL)){
		wstring errormsg((PTSTR)hlocal);
		return errormsg;
	}else{
		return TEXT("No text found for this error code.");
	}
}

std::wstring CErrorMsg::getMsg()
{
	DWORD error_code = GetLastError();
	return getMsg(error_code);
}