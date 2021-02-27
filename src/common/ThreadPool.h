#pragma once
#include <vector>
#include <thread>
#include "TQueue.h"

namespace Thread
{
	class Task
	{
	public:
		virtual ~Task();
		virtual void process() = 0;
		virtual void complete() = 0;
	};

	typedef std::shared_ptr<Task> TaskPtr;

	class ThreadPool;
	class CThread
	{
	public:
		CThread(ThreadPool * pool);
		virtual ~CThread();
		static void backfunc(CThread * t);
		void stop();

		virtual void run(TaskPtr task);

	protected:
		virtual void onStart() {};
		virtual void onEnd() {};

	protected:
		ThreadPool * _pool;
		std::thread _thread;
		std::atomic<bool> _isrun{ true };
	};

	class ThreadPool
	{
		friend CThread;
	public:
		ThreadPool();
		virtual ~ThreadPool();

		void create(int count);
		void exit();

		void addTask(TaskPtr task);
		void update();
	protected:
		virtual CThread* createThread() = 0;
		virtual void deleteThread(CThread * t) = 0;
		virtual void completeTask(TaskPtr task) = 0;
	private:
		TaskPtr popWaitTask();

	private:
		std::vector<CThread *> _threads;
		TQueue<TaskPtr> _waitTasks;
		TQueue<TaskPtr> _completeTasks;
	};
}

