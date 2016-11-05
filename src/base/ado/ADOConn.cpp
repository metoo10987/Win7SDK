#include "stdafx.h"
#include "ADOConn.h"
#include "ExceptionMgrImpl.h"


CADOConn::CADOConn(void):bConnectSucess(false)
{
	if(FAILED(::CoInitialize(NULL)))
	{
		CExceptionMgr::Instance()->WriteExceptionInfo("error_log","数据库环境环境初始化失败");
	}
	m_pADOConn.CreateInstance(__uuidof(Connection));
	m_pADOSet.CreateInstance(__uuidof(Recordset));
	m_pCommand.CreateInstance(__uuidof(Command));
}

CADOConn::~CADOConn(void)
{
	if(m_pADOSet->State==adStateOpen)
		m_pADOSet->Close();
	if(m_pADOConn->State==adStateOpen)
		m_pADOConn->Close();
}

bool CADOConn::ConnectDB()
{
	return false;
	try
	{
		//m_pADOConn->Open("Provider=SQLOLEDB;Persist Security Info=False;User ID=Sa;Initial Catalog=DETSDB;Data Source=192.168.0.120","","iddqd",adConnectUnspecified);
		//m_pADOConn->Open("Provider=SQLOLEDB;Persist Security Info=False;User ID=Sa;Initial Catalog=DETSDB;Data Source=THINK-PC","","zhb5352",adConnectUnspecified);
		//m_pADOConn->Open("Provider=SQLOLEDB;Persist Security Info=False;User ID=Sa;Initial Catalog=DETSDB;Data Source=zhaohaibin","","zhb5352",adConnectUnspecified);
		m_pADOConn->Open("Provider=SQLOLEDB;Persist Security Info=False;User ID=sa;Initial Catalog=DETSDB;Data Source=192.168.1.100","","iddqd_3852",adConnectUnspecified);
		m_pCommand->ActiveConnection=m_pADOConn;
		m_pCommand ->CommandType = adCmdStoredProc;

	}
	catch(_com_error &e)
	{
		//string error=e.Description();
		CExceptionMgr::Instance()->WriteExceptionInfo("error_log","ADOConn.cpp37:数据库连接失败"/*+error*/);
		bConnectSucess=false;
		return false;
	}
	bConnectSucess=true;
	return true;
}

bool CADOConn::IsEnd()
{
	if(!bConnectSucess)
		return true;
	if(!m_pADOSet->adoEOF)
		return false;
	else
		return true;
}

bool CADOConn::ADOExecute( string sql )
{
	if(!bConnectSucess)
		return false;
	if(m_pADOSet->State==adStateOpen)
		m_pADOSet->Close();
	try
	{
		m_pADOSet->Open((_variant_t)sql.c_str(),m_pADOConn.GetInterfacePtr(),adOpenStatic,adLockOptimistic,adCmdUnknown);
	}
	catch(_com_error &e)
	{
		string error=e.Description();
		CExceptionMgr::Instance()->WriteExceptionInfo("error_log","ADOConn:67,"+error);
		return false;
	}
	return true;
}

void CADOConn::MoveNext()
{
	m_pADOSet->MoveNext();
}

string CADOConn::GetItem(string Field_Name)
{
	string item;
	_bstr_t result;
	variant_t vt;
	try
	{
		vt=m_pADOSet->GetCollect(Field_Name.c_str());//参数为要取得值的字段名
		if(vt.vt==VT_NULL || vt.vt==VT_EMPTY)//检查结果是否为空或NULL
			return "";
		else
		{
			result=vt;
		}
	}
	catch(_com_error &e)
	{
		string error=e.Description();
		CExceptionMgr::Instance()->WriteExceptionInfo("error_log","CADOConn.cpp:r77,"+error);
		return "";
	}
	item=result;
	return item;
}

int CADOConn::GetItem( string Field_Name,char* p )
{
	_variant_t result;
	long bSize=-1;
	try
	{
		bSize=m_pADOSet->GetFields()->GetItem(Field_Name.c_str())->ActualSize;
		result=m_pADOSet->GetFields()->GetItem(Field_Name.c_str())->GetChunk(bSize);
	}
	catch(_com_error &e)
	{
		string error=e.Description();
		CExceptionMgr::Instance()->WriteExceptionInfo("error_log","读取字段："+Field_Name+"发生错误，错误信息："+error);
		return -1;
	}
	char* pImg=NULL;
	SafeArrayAccessData(result.parray,(void**)&pImg);
	memcpy(p,pImg,bSize);
	return bSize;
}

bool CADOConn::ADOExPro( string proname,string paremater )
{
	if(!bConnectSucess)
		return false;
	m_pCommand->CommandText=_bstr_t(proname.c_str());  //SP Name
	m_pCommand->Parameters->Refresh(); 
	m_pCommand->Parameters->Item[_variant_t(_bstr_t("@i_seq_name") )]->Value=_variant_t(paremater.c_str());
	_RecordsetPtr tempsetptr;
	tempsetptr.CreateInstance(__uuidof(Recordset));
	tempsetptr = m_pCommand->Execute(NULL,NULL,adCmdStoredProc);
	if(tempsetptr->State==adStateOpen)
		tempsetptr->Close();
	_variant_t   VretVal;
	VretVal = m_pCommand->Parameters->GetItem(short(2))->Value;
	int n=-1;
	n=VretVal.lVal;
	return true;
}

bool CADOConn::ADOExPro( string proname,string p0,int& p1 )
{
	if(!bConnectSucess)
		return false;
	m_pCommand->CommandText=_bstr_t(proname.c_str());  //SP Name
	m_pCommand->Parameters->Refresh(); 
	m_pCommand->Parameters->Item[_variant_t(_bstr_t("@i_seq_name") )]->Value=_variant_t(p0.c_str());
	_RecordsetPtr tempsetptr;
	tempsetptr.CreateInstance(__uuidof(Recordset));
	tempsetptr = m_pCommand->Execute(NULL,NULL,adCmdStoredProc);
	if(tempsetptr->State==adStateOpen)
		tempsetptr->Close();
	_variant_t   VretVal;
	VretVal = m_pCommand->Parameters->GetItem(short(2))->Value;
	int n=-1;
	p1=VretVal.lVal;
	return true;
}

bool CADOConn::ADOExPro( string proname,string in_p0,string in_p1,int& out_p2 )
{
	if(!bConnectSucess)
		return false;
	m_pCommand->CommandText=_bstr_t(proname.c_str());  //SP Name
	m_pCommand->Parameters->Refresh(); 
	m_pCommand->Parameters->Item[_variant_t(_bstr_t("@i_examr_id") )]->Value=_variant_t(in_p0.c_str());
	m_pCommand->Parameters->Item[_variant_t(_bstr_t("@i_rc_id") )]->Value=_variant_t(in_p1.c_str());
	_RecordsetPtr tempsetptr;
	tempsetptr.CreateInstance(__uuidof(Recordset));
	tempsetptr = m_pCommand->Execute(NULL,NULL,adCmdStoredProc);
	if(tempsetptr->State==adStateOpen)
		tempsetptr->Close();
	_variant_t   VretVal;
	VretVal = m_pCommand->Parameters->GetItem(short(3))->Value;
	int n=-1;
	out_p2=VretVal.lVal;
	return true;
}

bool CADOConn::ADOExPro( string proname,vector<tuple<string,string>> in_param,vector<string>& out_param ,int out_count)
{
	if(!bConnectSucess)
		return false;

	m_pCommand->CommandText=_bstr_t(proname.c_str());  //SP Name
	m_pCommand->Parameters->Refresh(); 
	int n=in_param.size();
	for(int i=0;i<n;++i)
	{
		try
		{
			m_pCommand->Parameters->Item[_variant_t(_bstr_t(in_param.at(i).get<0>().c_str()))]->Value=_variant_t(in_param.at(i).get<1>().c_str());
	
		}
		catch(_com_error &e)
		{
			string error=e.Description();
			CExceptionMgr::Instance()->WriteExceptionInfo("error_log","CADOConn.cpp:r178,"+error);
		}
	}
	_RecordsetPtr tempsetptr;
	tempsetptr.CreateInstance(__uuidof(Recordset));
	tempsetptr = m_pCommand->Execute(NULL,NULL,adCmdStoredProc);
	if(tempsetptr->State==adStateOpen)
		tempsetptr->Close();
	int start_index=in_param.size();
	start_index+=1;
	for(int j=0;j<out_count;++j)
	{
		_variant_t   VretVal;
		VretVal = m_pCommand->Parameters->GetItem(short(j+start_index))->Value;
		string str= string((char*)_bstr_t(VretVal));
		out_param.push_back(str);
	}
	return true;
}

bool CADOConn::ADOExPro( string proname,vector<tuple<string,string>> in_param,vector<int>& out_param,int out_count )
{
	if(!bConnectSucess)
		return false;
	m_pCommand->CommandText=_bstr_t(proname.c_str());  //SP Name
	m_pCommand->Parameters->Refresh(); 
	int n=in_param.size();
	for(int i=0;i<n;++i)
	{
		try
		{
			m_pCommand->Parameters->Item[_variant_t(_bstr_t(in_param.at(i).get<0>().c_str()))]->Value=_variant_t(in_param.at(i).get<1>().c_str());
		}
		catch(_com_error &e)
		{
			string error=e.Description();
			CExceptionMgr::Instance()->WriteExceptionInfo("error_log","CADOConn.cpp:r203,"+error);
		}
	}
	_RecordsetPtr tempsetptr;
	tempsetptr.CreateInstance(__uuidof(Recordset));
	tempsetptr = m_pCommand->Execute(NULL,NULL,adCmdStoredProc);
	if(tempsetptr->State==adStateOpen)
		tempsetptr->Close();
	int start_index=in_param.size();
	start_index+=1;
	for(int j=0;j<out_count;++j)
	{
		_variant_t   VretVal;
		VretVal = m_pCommand->Parameters->GetItem(short(j+start_index))->Value;
		int temp=VretVal.lVal;
		out_param.push_back(temp);
	}
	return true;
}
