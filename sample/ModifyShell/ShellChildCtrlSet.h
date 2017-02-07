#pragma once;
#include <string>
#include <Windows.h>
using namespace std;

class ShellChildCtrlSet
{
public:
	ShellChildCtrlSet(HWND explorerHwnd);
	~ShellChildCtrlSet();

public:
	static BOOL CALLBACK HandleEnumHwnd(HWND hwnd,LPARAM lparam);
private:
	void _initialize();
public:
	HWND m_explorerHwnd;

	HWND m_shellTableWindowHwnd;		//ShellTabWindowClass
	HWND m_shellTab_duiview;					// ShellTabWindowClass->DUIViewWndClassName
	HWND m_duiview_duihwnd;					//ShellTabWindowClass->DUIViewWndClassName->DirectUIHWND

	HWND m_ctrl_shelldllDefView;
	HWND m_ctrl_shelldllDefView_duihwnd;
};