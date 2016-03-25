#include "SingleThreadWorker.h"

WorkerSemaphore::WorkerSemaphore() :m_count(0)
{

}

WorkerSemaphore::~WorkerSemaphore()
{

}

void WorkerSemaphore::notify()
{
	count_increment();
	m_cond.notify_one();
}

bool WorkerSemaphore::time_wait( int seconds )
{
	if(m_count > 0)
	{
		count_decrement();
		return true;
	}
	boost::unique_lock<boost::mutex> lock(m_core_mut);
	if( m_cond.timed_wait(lock,boost::posix_time::seconds(seconds)) )
	{
		count_decrement();
		return true;
	}
	return false;
}

void WorkerSemaphore::count_increment()
{
	boost::mutex::scoped_lock lock(m_mut);
	++m_count;
}

void WorkerSemaphore::count_decrement()
{
	boost::mutex::scoped_lock lock(m_mut);
	--m_count;
}


WorkOperation::WorkOperation()
	: m_isCancle(false)
{

}

WorkOperation::~WorkOperation()
{

}

void WorkOperation::run()
{

}

void WorkOperation::cancle()
{
	m_isCancle = true;    
}

bool WorkOperation::isCancle()
{
	return m_isCancle;
}

CancleSuccessedOperation::CancleSuccessedOperation(std::function<void(void)> notifyCmd)
	: m_notifyCmd(notifyCmd)
{
}

CancleSuccessedOperation::~CancleSuccessedOperation()
{

}

void CancleSuccessedOperation::run()
{
	if(m_notifyCmd)
		m_notifyCmd();
}


SigleThreadWorker::SigleThreadWorker() :m_cancle(false)
{

}

SigleThreadWorker::SigleThreadWorker( const std::string& name )
	: m_name(name)
	, m_cancle(false)
{

}

SigleThreadWorker::~SigleThreadWorker()
{

}

void SigleThreadWorker::postWork( WorkOperationSharedPtr spOperation )
{
	boost::mutex::scoped_lock lock(m_operationQueueOptMutex);
	m_operationQueue.push_back(spOperation);
	m_semaphore.notify();
}

void SigleThreadWorker::workThread()
{
	for(;;)
	{
		if( cancle() )
			return;
		if(m_semaphore.time_wait(1))
		{
			WorkOperationSharedPtr spOperation = getOperation();
			if(spOperation)
			{
				if(cancle())
					return;
				spOperation->run();
				popOperation();
			}
		}
	}
}

void SigleThreadWorker::stop()
{
	m_cancle = true;
	stopAllOperation();
	m_workThread.join();
}

void SigleThreadWorker::run()
{
	m_workThread = boost::thread(&SigleThreadWorker::workThread, this);
}

bool SigleThreadWorker::cancle()
{
	return m_cancle;
}

void SigleThreadWorker::cancleAllOperation()
{
	boost::mutex::scoped_lock lock(m_operationQueueOptMutex);
	std::deque<WorkOperationSharedPtr >::iterator it = m_operationQueue.begin();
	for(; it != m_operationQueue.end(); ++it)
	{
		(*it)->cancle();
        (*it)->post_cancel();
	}

	WorkOperationSharedPtr spOperation(new CancleSuccessedOperation(std::bind(&SigleThreadWorker::allOperationCancleSuccessed,this)));
	m_operationQueue.push_back(spOperation);
	m_semaphore.notify();
}

WorkOperationSharedPtr SigleThreadWorker::getOperation()
{
	boost::mutex::scoped_lock lock(m_operationQueueOptMutex);
	if(m_operationQueue.empty())
		return NULL;

	WorkOperationSharedPtr spOperation = m_operationQueue.front();
	return spOperation;
}

void SigleThreadWorker::popOperation()
{
	boost::mutex::scoped_lock lock(m_operationQueueOptMutex);
	m_operationQueue.pop_front();
}

void SigleThreadWorker::allOperationCancleSuccessed()
{
	if(m_cancleSuccessedNotifyCmd)
		m_cancleSuccessedNotifyCmd();
}

void SigleThreadWorker::registerCancleSuccessedNotify( std::function<void(void)> notifyCmd )
{
	m_cancleSuccessedNotifyCmd = notifyCmd;
}

void SigleThreadWorker::stopAllOperation()
{
	boost::mutex::scoped_lock lock(m_operationQueueOptMutex);
	std::deque<WorkOperationSharedPtr >::iterator it = m_operationQueue.begin();
	for(; it != m_operationQueue.end(); ++it)
	{
		(*it)->cancle();
	}
}




