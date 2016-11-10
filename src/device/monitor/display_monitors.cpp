#include "display_monitors.h"

BOOL CALLBACK MonitorEnumProc( HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	display_monitors* p_monitors = (display_monitors*) dwData;
	MONITORINFO monitor;
	monitor.cbSize = sizeof(MONITORINFO);
	
	//获取显示器信息
	GetMonitorInfo(hMonitor, &monitor);
	p_monitors->push_monitor(monitor);
	return TRUE;
}

display_monitors::display_monitors(void)
{
}

display_monitors::~display_monitors(void)
{
}

void display_monitors::push_monitor( MONITORINFO& monitor )
{
	m_monitors_list.push_back(monitor);
}

void display_monitors::initialize_system_monitors()
{
	EnumDisplayMonitors(NULL,NULL,MonitorEnumProc,(LPARAM)this);
}

bool display_monitors::get_monitor( int num, MONITORINFO& monitor )
{
	if( num > m_monitors_list.size() || num < 1)
		return false;
	monitor = m_monitors_list[num - 1];
	return true;
}
