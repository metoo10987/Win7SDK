#pragma once
#define  COMPILE_MULTIMON_STUBS
#include <Windows.h>
#include <vector>
using namespace std;

class display_monitors
{
public:
	display_monitors(void);
	~display_monitors(void);
	void push_monitor(MONITORINFO& monitor);
	void initialize_system_monitors();
	bool get_monitor(int num, MONITORINFO& monitor);
private:
	vector<MONITORINFO> m_monitors_list;
};
