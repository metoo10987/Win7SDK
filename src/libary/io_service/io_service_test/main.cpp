#include <Windows.h>
#include <iostream>
using namespace std;
#include "../io_service/io_service.h"


void show(string str)
{
	cout << str << endl;
}

int main()
{
	init_io_serviece(2);
	std::tr1::function<void(string)> cmd = std::bind(show,std::tr1::placeholders::_1);
	
	int ncount = 0;
	for(int i = 1; i < 100000; ++i)
	{
		char sz_num[6];
		itoa(i, sz_num, 10);
		string str = string(sz_num);
		str += "：测试数据";
		if(!post_commond(&cmd,str))
		{
			cout << "------------------------------" << "消息队列已满，投递失败" << " --------------------------------" << endl;
			++ncount;
		}
	}
	char c;
	cin >> c;
	for(int i = 1; i < 100000; ++i)
	{
		char sz_num[6];
		itoa(i, sz_num, 10);
		string str = string(sz_num);
		str += "：测试数据";
		if(!post_commond(&cmd,str))
		{
			cout << "------------------------------" << "消息队列已满，投递失败" << " --------------------------------" << endl;
			++ncount;
		}
	}

	int n;
	cin >> n;
	cout << "完成" << endl;
	cout << "投递失败数量："<< ncount << endl;
	
	return 0;
}