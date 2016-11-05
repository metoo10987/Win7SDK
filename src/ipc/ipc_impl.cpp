#include "ipc_impl.h"
#include <iostream>
#include <component/any/any.hpp>

ipc_impl::ipc_impl() : m_buffer_size(1024)
{
	InitializeCriticalSection(&m_cs);
	m_cmd = tr1::bind(&ipc_impl::handle, this, tr1::placeholders::_1);
}

ipc_impl::ipc_impl( int buffer_size ) : m_buffer_size(buffer_size)
{
	InitializeCriticalSection(&m_cs);
	m_cmd = tr1::bind(&ipc_impl::handle, this, tr1::placeholders::_1);
}

ipc_impl::~ipc_impl( void )
{
}

void ipc_impl::listen( string name)
{
	InitWRCommunicate(name,m_buffer_size,true);
}

void ipc_impl::connect( string name)
{
	InitWRCommunicate(name,m_buffer_size);
}

void ipc_impl::write( string wname,const char* p_src, int langth )
{
	EnterCriticalSection(&m_cs);
	ipc_msg msg;
	msg.body_length(langth);
	memcpy(msg.body(), p_src, msg.body_length());
	msg.encode();
	map<string,read_write_impl*>::iterator it = m_pCommunicateSet.find(wname);
	if(it != m_pCommunicateSet.end()){
		it->second->write(msg);
	}
	LeaveCriticalSection(&m_cs);
}

void ipc_impl::close( string name )
{
	EnterCriticalSection(&m_cs);
	map<string,read_write_impl*>::iterator it = m_pCommunicateSet.find(name);
	if(it != m_pCommunicateSet.end()){
		it->second->close();
		delete (it->second);
		m_pCommunicateSet.erase(it);
	}
	LeaveCriticalSection(&m_cs);
}

void ipc_impl::close_all()
{
	map<string,read_write_impl*>::iterator it = m_pCommunicateSet.begin();
	while(it != m_pCommunicateSet.end()){
		(it->second)->close();
		delete (it->second);
	}
	m_pCommunicateSet.clear();
}

DWORD WINAPI ipc_impl::ProcessReadThreadPro( LPVOID lpParameter )
{
	ipc_impl* p = (ipc_impl*)lpParameter;
	read_write_impl* pCommunicate = p->getnewCommunicate();
	if(pCommunicate == NULL)
		return 0;
	while(!pCommunicate->isEnd())
	{
		ipc_msg* p_msg = pCommunicate->read();
		p->submit_work(p_msg);
	}
	return 0;
}

read_write_impl* ipc_impl::getnewCommunicate()
{
	if(m_wait_deal.size() > 0){
		EnterCriticalSection(&m_cs);
		string name = m_wait_deal.front();
		m_wait_deal.pop_front();
		map<string,read_write_impl*>::iterator it = m_pCommunicateSet.find(name);
		if(it != m_pCommunicateSet.end()){
			read_write_impl* pwr = it->second;
			LeaveCriticalSection(&m_cs);
			return pwr;
		}
	}
	return NULL;
}

void ipc_impl::handle( any& msg )
{
	ipc_msg _msg = any_cast<ipc_msg>(msg);
	m_handle(_msg.body(),_msg.body_length());
}

void ipc_impl::submit_work( ipc_msg* p_msg )
{
	any obj;
	obj = (*p_msg);
	m_workhandle.submit_work(m_cmd, obj);
	delete p_msg;
}

void ipc_impl::InitWRCommunicate( string name,int buffersize,bool bRead /*= false*/ )
{
	EnterCriticalSection(&m_cs);
	m_wait_deal.push_back(name);
	read_write_impl* p = new read_write_impl(name,buffersize);
	m_pCommunicateSet.insert(map<string,read_write_impl*>::value_type(name,p));
	LeaveCriticalSection(&m_cs);
	if(bRead){
		CreateThread(NULL,0,&ipc_impl::ProcessReadThreadPro,
			(LPVOID)this,0,NULL);
	}
}

void ipc_impl::register_listen_handler( handle_type& handler )
{
	m_handle = handler;
}
