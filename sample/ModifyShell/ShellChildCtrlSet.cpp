#include "ShellChildCtrlSet.h"



ShellChildCtrlSet::ShellChildCtrlSet(HWND explorerHwnd)
	: m_explorerHwnd(explorerHwnd)
{
	_initialize();
}

ShellChildCtrlSet::~ShellChildCtrlSet()
{

}

void ShellChildCtrlSet::_initialize()
{
	m_shellTableWindowHwnd = FindWindowEx(m_explorerHwnd, 0, L"ShellTabWindowClass", NULL);
	m_shellTab_duiview = FindWindowEx(m_shellTableWindowHwnd, 0, L"DUIViewWndClassName", NULL);
	m_duiview_duihwnd = FindWindowEx(m_shellTab_duiview, 0, L"DirectUIHWND", NULL);

	EnumChildWindows(m_shellTab_duiview, HandleEnumHwnd, (LPARAM)this);
	m_ctrl_shelldllDefView_duihwnd = FindWindowEx(m_ctrl_shelldllDefView, 0, L"DirectUIHWND", NULL);
}

BOOL CALLBACK ShellChildCtrlSet::HandleEnumHwnd( HWND hwnd,LPARAM lparam )
{
	ShellChildCtrlSet* pThis = (ShellChildCtrlSet*)lparam;
	HWND hTemp = FindWindowEx(hwnd, 0, L"SHELLDLL_DefView", NULL);
	if(hTemp)
	{
		pThis->m_ctrl_shelldllDefView = hTemp;
		return FALSE;
	}
	return TRUE;
}

