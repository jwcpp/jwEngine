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
class RedisResult;
class SqlPrepare;
class SqlResultSet;

class DBTask : public Task
{
public:
	DBTask();
	~DBTask();

	void dbi(DB_Interface * dbi);

protected:
	DB_Interface * _dbi;
	int _ret;
	int32 _errno = 0;
	std::string _error;
};

class DBSqlTask : public DBTask
{
public:
	DBSqlTask(std::shared_ptr<SqlPrepare> pre, std::shared_ptr <SqlResultSet> result);
	~DBSqlTask();

	virtual void process();
	virtual void complete();
public:
	std::function<void(int32, const char*, std::shared_ptr<SqlResultSet>)> backfunc;
private:
	std::shared_ptr<SqlPrepare> _pre;
	std::shared_ptr <SqlResultSet> _result;
};

class DBRedisTask : public DBTask
{
public:
	DBRedisTask(std::shared_ptr<RedisCommand> command, std::shared_ptr <RedisResult> result);
	~DBRedisTask();

	virtual void process();
	virtual void complete();
public:
	std::function<void(int32, const char*, std::shared_ptr<RedisResult>)> backfunc;
private:
	std::shared_ptr<RedisCommand> _command;
	std::shared_ptr<RedisResult> _result;
};

class DBThread : public CThread
{
public:
	DBThread(ThreadPool * pool);
	~DBThread();
	virtual void onStart();
	virtual void onEnd();

	virtual void run(TaskPtr task);

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
	virtual void completeTask(TaskPtr task);

	const DBConfig * getConfig();

private:
	DBConfig m_config;
};

