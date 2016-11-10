#pragma once
//�洢���̵�ִ�У�
//��ִ�д洢���̺�ȡ����ֵʱ�����ղ������Ⱥ�˳����±�0��ʼ
//�����±�0���ǻ�ֵ�������������մ洢���̴�1��ʼ�������ƣ���������������������������
#import "c:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF") rename("BOF","adoBOF")
#include <string>
#include <loki/Singleton.h>
#include <boost/bind.hpp>
#include <boost/tuple/tuple.hpp>
#include <vector>

using namespace boost;
using namespace std;

class CADOConn
{
public:
	CADOConn(void);
	~CADOConn(void);
private:
	_ConnectionPtr m_pADOConn;
	_RecordsetPtr m_pADOSet;
	_CommandPtr m_pCommand;
	bool bConnectSucess;
public:
	bool ConnectDB();
	bool ADOExecute(string sql);		//ִ��SQL���
	bool IsEnd();						//�жϼ�¼���α��Ƿ��ĩβ
	void MoveNext();					//��¼���α�����
	string GetItem(string Field_Name);	//ȡ���ֶ�ֵ
	int GetItem(string Field_Name,char* p);
	bool ADOExPro(string proname){return false;}		//ִ���޲����Ĵ洢����
	bool ADOExPro(string proname,string paremater);
	bool ADOExPro(string proname,string p0,int& p1);
	bool ADOExPro(string proname,string in_p0,string in_p1,int& out_p2);
	bool ADOExPro(string proname,vector<tuple<string,string>> in_param,vector<string>& out_param,int out_count);
	bool ADOExPro(string proname,vector<tuple<string,string>> in_param,vector<int>& out_param,int out_count);
};
typedef Loki::SingletonHolder<CADOConn> CADoMgr;
