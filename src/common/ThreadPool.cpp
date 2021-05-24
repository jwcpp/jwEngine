#include "ThreadPool.h"
#include <thread>

using namespace Thread;

Task::~Task()
{

}

CThread::CThread(ThreadPool * pool) :
	_pool(pool),
	_thread(CThread::backfunc, this)
{
	//_thread.detach();
}

CThread::~CThread()
{

}

void CThread::backfunc(CThread * t)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	t->onStart();
	while (t->_isrun)
	{
		TaskPtr task = t->_pool->popWaitTask();
		if (task)
		{
			t->run(task);
			t->_pool->_completeTasks.push(task);
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
	t->onEnd();
}

void CThread::stop()
{
	_isrun = false;
	_thread.join();
}

void CThread::run(TaskPtr task)
{
	task->process();
};

ThreadPool::ThreadPool()
{
	
}


ThreadPool::~ThreadPool()
{
	exit();
}

void ThreadPool::create(int count)
{
	for (int i = 0; i < count; ++i)
	{
		CThread * t = createThread();
		if (t)
		{
			_threads.push_back(t);
		}
	}
}
void ThreadPool::exit()
{
	for (CThread * t : _threads)
	{
		t->stop();
		deleteThread(t);
	}

	_threads.clear();
}

void ThreadPool::addTask(TaskPtr task)
{
	_waitTasks.push(task);
}

TaskPtr ThreadPool::popWaitTask()
{
	TaskPtr task;
	if (!_waitTasks.tryPop(task))
		return NULL;
	return task;
}

void ThreadPool::update()
{
	TaskPtr task;
	if (!_completeTasks.tryPop(task))
		return;

	task->complete();
	completeTask(task);
}