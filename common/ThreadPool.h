#pragma once
#include <vector>
#include <thread>
#include "TQueue.h"

namespace Thread
{
	class Task
	{
	public:
		virtual void process() = 0;
		virtual void complete() = 0;
	};

	class ThreadPool;
	class CThread
	{
	public:
		CThread(ThreadPool * pool);
		static void backfunc(CThread * t);
		void stop();

		virtual void run(Task * task) { task->process(); };

	protected:
		virtual void onStart() {};
		virtual void onEnd() {};

	private:
		ThreadPool * _pool;
		std::thread _thread;
		std::atomic<bool> _isrun{ true };
	};

	class ThreadPool
	{
		friend CThread;
	public:
		ThreadPool();
		~ThreadPool();

		void create(int count);
		void exit();

		void addTask(Task * task);
		void update();
	protected:
		virtual CThread* createThread() { return NULL; }
		virtual void deleteThread(CThread * t) = 0;
		virtual void completeTask(Task * task) = 0;
	private:
		Task * popWaitTask();

	private:
		std::vector<CThread *> _threads;
		TQueue<Task *> _waitTasks;
		TQueue<Task *> _completeTasks;
	};
}

