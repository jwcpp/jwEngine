#include <string>
#include "BaseType.h"
#include "mysql.h"
#include "DB_Interface_mysql.h"
#include "hiredis.h"
#include "DB_Interface_redis.h"
#include "SqlPrepare.h"
#include <functional>
#include "DBThreadPool.h"


DBTask::DBTask()
{
	
}

DBTask::~DBTask()
{
}

void DBTask::dbi(DB_Interface * dbi)
{
	_dbi = dbi;
}

//------------------------------------------------

DBSqlTask::DBSqlTask(std::shared_ptr<SqlPrepare> pre, std::shared_ptr <SqlResultSet> result):
	_pre(pre), _result(result)
{

}

DBSqlTask::~DBSqlTask()
{

}

void DBSqlTask::process()
{
	_ret = _pre->prepare(static_cast<DBInterfaceMysql *>(_dbi)->mysql());
	DBResult* result = (DBResult*)(_result.get());
	if (_ret >= 0) _ret = _pre->execute(result);
	if (_ret < 0)
	{
		_errno = _dbi->getErrno();
		_error = _dbi->getError();
	}
}

void DBSqlTask::complete()
{
	const char* str = NULL;
	if (_ret < 0) str = _error.c_str();
	if (backfunc)
	{
		backfunc(_errno, str, _result);
		backfunc = nullptr;
	}
}

//------------------------------------------------

DBRedisTask::DBRedisTask(std::shared_ptr<RedisCommand> command, std::shared_ptr <RedisResult> result)
{
	_command = command;
	_result = result;
}

DBRedisTask::~DBRedisTask()
{

}

void DBRedisTask::process()
{
	DBResult* result = (DBResult*)(_result.get());
	_ret = static_cast<DBInterfaceRedis *>(_dbi)->execute(_command.get(), result);
	if (_ret < 0)
	{
		_errno = _dbi->getErrno();
		_error = _dbi->getError();
	}
}

void DBRedisTask::complete()
{
	const char* str = NULL;
	if (_ret < 0) str = _error.c_str();
	if (backfunc)
	{
		backfunc(_errno, str, _result);
		backfunc = nullptr;
	}
}

//------------------------------------------------

DBThread::DBThread(ThreadPool * pool):
	CThread(pool)
{
	m_db = NULL;
}

DBThread::~DBThread()
{

}

void DBThread::onStart()
{
	DBThreadPool * pool = static_cast<DBThreadPool*>(_pool);

	const DBConfig * config = pool->getConfig();
	if (config->device == "mysql")
	{
		m_db = new DBInterfaceMysql(config->ip.c_str(), config->dbname.c_str(), config->user.c_str(), config->pswd.c_str(), config->port);
	}
	else
	{
		m_db = new DBInterfaceRedis(config->ip.c_str(), config->port);
	}
	
	m_db->connect();
}

void DBThread::onEnd()
{
	if (m_db)
	{
		delete m_db;
	}
}

void DBThread::run(TaskPtr task)
{
	static_cast<DBTask*>(task.get())->dbi(m_db);
	CThread::run(task);
}

DBThreadPool::DBThreadPool(DBConfig config)
{
	m_config = config;
}

DBThreadPool::~DBThreadPool()
{
	
}

CThread* DBThreadPool::createThread()
{
	return new DBThread(this);
}

void DBThreadPool::deleteThread(CThread * t)
{
	delete t;
}

void DBThreadPool::completeTask(TaskPtr task)
{
}

const DBConfig * DBThreadPool::getConfig()
{
	return &m_config;
}
