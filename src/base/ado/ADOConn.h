#pragma once
//存储过程的执行：
//在执行存储过程后取返回值时，按照参数的先后顺序从下标0开始
//但是下标0总是回值，其他参数依照存储过程从1开始依次类推（参数包含输入参数和输出参数）
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
	bool ADOExecute(string sql);		//执行SQL语句
	bool IsEnd();						//判断记录集游标是否的末尾
	void MoveNext();					//记录集游标下移
	string GetItem(string Field_Name);	//取得字段值
	int GetItem(string Field_Name,char* p);
	bool ADOExPro(string proname){return false;}		//执行无参数的存储过程
	bool ADOExPro(string proname,string paremater);
	bool ADOExPro(string proname,string p0,int& p1);
	bool ADOExPro(string proname,string in_p0,string in_p1,int& out_p2);
	bool ADOExPro(string proname,vector<tuple<string,string>> in_param,vector<string>& out_param,int out_count);
	bool ADOExPro(string proname,vector<tuple<string,string>> in_param,vector<int>& out_param,int out_count);
};
typedef Loki::SingletonHolder<CADOConn> CADoMgr;
