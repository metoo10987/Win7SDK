#pragma once
#include <Windows.h>
#include <string>
using namespace std;
//该类封装了Win32信号量同步机制，支持线程、进程间同步
//信号量同步机制，支持一个线程发送信号，一个线程等待信号,每发送一个信号
//内核对象的信号计数加1，每等待到一个信号内核对象信号计数减1，当内核对象
//信号计数为0时，等待操作将被阻塞，直到信号计数大于0或等待超时

//CreateSemaphore创建一个信号量内核对象
// HANDLE WINAPI CreateSemaphore(
// 	_In_opt_  LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,	//安全属性
// 	_In_      LONG lInitialCount,							//创建内核对象，该对象上的信号计数
// 	_In_      LONG lMaximumCount,							//最大信号计数数量
// 	_In_opt_  LPCTSTR lpName								//内核对象名称
// 	);

class semaphore_impl
{
public:
	semaphore_impl(int usercount, int maxcount,string name);
	~semaphore_impl(void);
	void SetImpl(int n = 1);						//信号计数加1
	void WaitImpl(long lMilliseconds = INFINITE);	//等待信号，信号计数减1
	void CloseImpl();
private:
	HANDLE m_hSemaphore;
};