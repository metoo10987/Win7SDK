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
	//����������ʶ
	//��LANG_NEUTRAL��SUBLANG_NEUTRAL���ɲ���ϵͳĬ������
	DWORD systemLocale = MAKELANGID(LANG_NEUTRAL,SUBLANG_NEUTRAL);
	BOOL fok = FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM |//ȡ��ϵͳ����Ĵ�������Ӧ���ַ���
		FORMAT_MESSAGE_IGNORE_INSERTS |//�����ú���%վλ������Ϣ����Щռλ�������ṩ���������������Ϣ
		FORMAT_MESSAGE_ALLOCATE_BUFFER,//Ҫ��������һ���������ɴ����ı��������ڴ棬����ڴ�ľ������hlocal�з���
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