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
		str += "����������";
		if(!post_commond(&cmd,str))
		{
			cout << "------------------------------" << "��Ϣ����������Ͷ��ʧ��" << " --------------------------------" << endl;
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
		str += "����������";
		if(!post_commond(&cmd,str))
		{
			cout << "------------------------------" << "��Ϣ����������Ͷ��ʧ��" << " --------------------------------" << endl;
			++ncount;
		}
	}

	int n;
	cin >> n;
	cout << "���" << endl;
	cout << "Ͷ��ʧ��������"<< ncount << endl;
	
	return 0;
}