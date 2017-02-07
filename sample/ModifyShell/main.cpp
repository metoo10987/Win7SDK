#include "ShellChildCtrlSet.h"
#include "ModifyShellChildCtrlPos.h"

HWND GetForegroundExplorer()
{
	HWND hWnd = GetForegroundWindow();
	WCHAR Buffer[255] = {0};
	GetClassName(hWnd, Buffer, 255);
	if(wstring(Buffer) == L"CabinetWClass" || wstring(Buffer) == L"ExploreWClass")
		return hWnd;
	return NULL;
}
int main()
{
	for(;;)
	{
		HWND hExplorerWnd = GetForegroundExplorer();
		if(hExplorerWnd)
		{
			ShellChildCtrlSet shellChildCtrlSet(hExplorerWnd);
			ModifyExplorerChildCtrlPos(shellChildCtrlSet);
		}
		Sleep(1000);
	}
	return 0;
}