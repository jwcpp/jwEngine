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

struct DBConfig
{
	std::string device = "mysql"; // mysql or redis
	std::string ip = "local";
	unsigned int port = 3306;
	std::string dbname;
	std::string user = "root";
	std::string pswd;
};

class DB_Interface;
class RedisCommand;
class DBResult;
class SqlPrepare;

class DBTask : public Task
{
public:
	DBTask();
	~DBTask();

	void complete_back(std::function<void()> backfunc);
	void dbi(DB_Interface * dbi);
	virtual void complete();

protected:
	DB_Interface * _dbi;
	std::function<void()> _complete_back;
};

class DBSqlTask : public DBTask
{
public:
	DBSqlTask(SqlPrepare * pre);
	~DBSqlTask();

	virtual void process();

private:
	SqlPrepare * _pre;
};

class DBRedisTask : public DBTask
{
public:
	DBRedisTask(RedisCommand * command, DBResult * result);
	~DBRedisTask();

	virtual void process();
private:
	RedisCommand * _command;
	DBResult * _result;
};

class DBThread : public CThread
{
public:
	DBThread(ThreadPool * pool);
	~DBThread();
	virtual void onStart();
	virtual void onEnd();

	virtual void run(Task * task);

private:
	DB_Interface * m_db;
};

class DBThreadPool : public ThreadPool
{
public:
	DBThreadPool(DBConfig config);
	~DBThreadPool();
	virtual CThread* createThread();
	virtual void deleteThread(CThread * t);
	virtual void completeTask(Task * task);

	const DBConfig * getConfig();

private:
	DBConfig m_config;
};

