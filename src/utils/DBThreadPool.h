#pragma once

/************************************************************************
* @file      DBThreadPool.h
* @brief     mysql thread pool
* @author    jiangwang
* @data      2020-3-25
* @version   0.1
************************************************************************/

#include "ThreadPool.h"
using namespace Thread;

class DBTask : public Task
{
public:
	DBTask(SqlPrepare * pre);
	~DBTask();

	void complete_back(std::function<void()> backfunc);
	virtual void process();
	virtual void complete();
	void mysql(MYSQL * sql);
	SqlPrepare * prepare();

private:
	MYSQL * _mysql;
	SqlPrepare * _pre;
	std::function<void()> _complete_back;
};

class DBInterfaceMysql;
class DBThread : public CThread
{
public:
	DBThread(ThreadPool * pool);
	~DBThread();
	virtual void onStart();
	virtual void onEnd();

	virtual void run(Task * task);

private:
	DBInterfaceMysql * m_db;
};

class DBThreadPool : public ThreadPool
{
public:
	DBThreadPool(const char * host, const char * dbname, const char * user, const char * pswd = "", unsigned int port = 3306);
	~DBThreadPool();
	virtual CThread* createThread();
	virtual void deleteThread(CThread * t);
	virtual void completeTask(Task * task);

	const char * host();
	const char * dbname();
	const char * user();
	const char * pswd();
	unsigned int port();

private:
	std::string _host;
	std::string _dbname;
	std::string _user;
	std::string _pswd;
	unsigned int _port;
};

