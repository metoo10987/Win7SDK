#ifndef SIGNLE_THREAD_WORKER_H
#define SIGNLE_THREAD_WORKER_H

#include <memory>
#include <queue>
#include <functional>

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>

class WorkerSemaphore
{
public:
	WorkerSemaphore();
	~WorkerSemaphore();
public:
	void notify();
	bool time_wait(int seconds);

private:
	void count_increment();
	void count_decrement();
private:
	unsigned int m_count;
	boost::mutex m_mut;
	boost::mutex m_core_mut;
	boost::condition_variable m_cond;
};

class SigleThreadWorker;
class WorkOperation
{
	friend class SigleThreadWorker;
public:
	WorkOperation();
	virtual ~WorkOperation();
	virtual bool isCancle();
protected:
	virtual void run();
	virtual void cancle();
    virtual void post_cancel(){};
    
protected:
	bool m_isCancle;
};


class CancleSuccessedOperation : public WorkOperation
{
public:
	CancleSuccessedOperation(std::function<void(void)> notifyCmd);
	~CancleSuccessedOperation();
protected:
	virtual void run();
private:
	std::function<void(void)> m_notifyCmd;
};

typedef std::shared_ptr<WorkOperation> WorkOperationSharedPtr;

class SigleThreadWorker
{
public:
	SigleThreadWorker();
	SigleThreadWorker(const std::string& name);
	~SigleThreadWorker();

	void postWork(WorkOperationSharedPtr spOperation);
	void workThread();
	void stop();
	void run();
	void cancleAllOperation();
	void registerCancleSuccessedNotify(std::function<void(void)> notifyCmd);
private:
	bool cancle();
	WorkOperationSharedPtr getOperation();
	void popOperation();
	void stopAllOperation();
	void allOperationCancleSuccessed();
private:
	boost::mutex m_operationQueueOptMutex;
	WorkerSemaphore m_semaphore;
	std::deque<WorkOperationSharedPtr > m_operationQueue;
	std::function<void(void)> m_cancleSuccessedNotifyCmd;
	boost::thread m_workThread;
	bool m_cancle;
	std::string m_name;
};
#endif