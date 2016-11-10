#include <ipc/ipc.h>
#include <iostream>
#include <Windows.h>
using namespace std;

void show(char* psrc, int langth)
{
	char * pdesc = new char[langth+1];
	pdesc[langth] = '\0';
	memcpy(pdesc,psrc,langth);
	cout << pdesc << endl;
	delete [] pdesc;
}

int main()
{
	init_ipc();
	register_read_handler(tr1::bind(&show, tr1::placeholders::_1, tr1::placeholders::_2));
	string wname;
	cout << "请输入连接的名称：";
	cin >> wname;

	string rname;
	cout << "请输入监听的名称：";
	cin >> rname;

	start_listen(rname);

	to_connect(wname.c_str());
	char buf[10];

	
	for(;;)
	{
		string cmd;
		cin>> cmd;
		if(cmd == "exit")
			return 0;
		if(cmd == "xh")
		{
			for(int i = 1; i <= 10000000; ++i)
			{
				memset(buf,0,10);
				_itoa_s(i,buf,10);
				string str = buf;
				str+=":这是一条测试数据不用回复，赵海彬。";
				to_send(wname.c_str(),str.c_str(),str.size());
				Sleep(2);
			}
		}else
		{
			to_send(wname.c_str(),cmd.c_str(),cmd.size());
		}
	}
	
	
	return 0;
}