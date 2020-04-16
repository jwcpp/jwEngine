#include <string>
#include "BaseType.h"
#include "mysql.h"
#include "DB_Interface_mysql.h"
#include "hiredis.h"
#include "DB_Interface_redis.h"
#include "SqlPrepare.h"
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

void DBTask::complete_back(std::function<void()> backfunc)
{
	_complete_back = backfunc;
}

void DBTask::complete()
{
	if (_complete_back != nullptr)
		_complete_back();
}

//------------------------------------------------

DBSqlTask::DBSqlTask(SqlPrepare * pre):
	_pre(pre)
{

}

DBSqlTask::~DBSqlTask()
{

}

void DBSqlTask::process()
{
	_pre->prepare(static_cast<DBInterfaceMysql *>(_dbi)->mysql());
	_pre->execute();
}


//------------------------------------------------

DBRedisTask::DBRedisTask(RedisCommand * command, DBResult * result)
{
	_command = command;
	_result = result;
}

DBRedisTask::~DBRedisTask()
{

}

void DBRedisTask::process()
{
	static_cast<DBInterfaceRedis *>(_dbi)->execute(_command, _result);
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

void DBThread::run(Task * task)
{
	static_cast<DBTask*>(task)->dbi(m_db);
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

void DBThreadPool::completeTask(Task * task)
{
}

const DBConfig * DBThreadPool::getConfig()
{
	return &m_config;
}
