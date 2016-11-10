#pragma once
#include <component/any/any.hpp>

#include<Windows.h>
#include <string>
#include <functional>
#include <iostream>
using namespace std;

class work_pool
{
public:
	work_pool(void);
	~work_pool(void);
	static DWORD WINAPI thread_pool_callback(LPVOID pvContext);
	typedef tr1::function<void(any&)> cmdtype_1;
	void submit_work(cmdtype_1& cmd, any& obj);
	void close();
private:
	void close_environment();
private:
	struct work_item
	{
	public:
		work_item(cmdtype_1& cmd,any& obj);
		~work_item();
		cmdtype_1 cmd;
		any in_param;
	};
};
