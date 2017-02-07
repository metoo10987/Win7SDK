#include "ModifyShellChildCtrlPos.h"
void ModifyExplorerChildCtrlPos( const ShellChildCtrlSet& shellChildCtrlSet )
{
	RECT ctrl_shelldllDefViewRect;
	RECT ctrl_shelldllDefView_duihwndRect;

	GetWindowRect(shellChildCtrlSet.m_ctrl_shelldllDefView, &ctrl_shelldllDefViewRect);
	GetWindowRect(shellChildCtrlSet.m_ctrl_shelldllDefView_duihwnd, & ctrl_shelldllDefView_duihwndRect);

	LONG width = ctrl_shelldllDefViewRect.right - ctrl_shelldllDefViewRect.left - 10;
	LONG height = ctrl_shelldllDefViewRect.bottom - ctrl_shelldllDefViewRect.top;
	

	MoveWindow(shellChildCtrlSet.m_ctrl_shelldllDefView, 0,0, width, height, TRUE);
	MoveWindow(shellChildCtrlSet.m_ctrl_shelldllDefView_duihwnd, 0, 0, width, height, TRUE);
}

