#pragma once
#include <Windows.h>
#include <string>
using namespace std;
//�����װ��Win32�ź���ͬ�����ƣ�֧���̡߳����̼�ͬ��
//�ź���ͬ�����ƣ�֧��һ���̷߳����źţ�һ���̵߳ȴ��ź�,ÿ����һ���ź�
//�ں˶�����źż�����1��ÿ�ȴ���һ���ź��ں˶����źż�����1�����ں˶���
//�źż���Ϊ0ʱ���ȴ���������������ֱ���źż�������0��ȴ���ʱ

//CreateSemaphore����һ���ź����ں˶���
// HANDLE WINAPI CreateSemaphore(
// 	_In_opt_  LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,	//��ȫ����
// 	_In_      LONG lInitialCount,							//�����ں˶��󣬸ö����ϵ��źż���
// 	_In_      LONG lMaximumCount,							//����źż�������
// 	_In_opt_  LPCTSTR lpName								//�ں˶�������
// 	);

class semaphore_impl
{
public:
	semaphore_impl(int usercount, int maxcount,string name);
	~semaphore_impl(void);
	void SetImpl(int n = 1);						//�źż�����1
	void WaitImpl(long lMilliseconds = INFINITE);	//�ȴ��źţ��źż�����1
	void CloseImpl();
private:
	HANDLE m_hSemaphore;
};