#include <string>
#include "DB_Interface_mysql.h"
#include "BaseType.h"
#include "SqlPrepare.h"
#include "DBThreadPool.h"


DBTask::DBTask(SqlPrepare * pre):
	_pre(pre)
{
	
}

DBTask::~DBTask()
{
}

void DBTask::process()
{
	_pre->prepare(_mysql);
	_pre->execute();
}

void DBTask::complete()
{
	if (_complete_back != nullptr)
		_complete_back();
}

void DBTask::mysql(MYSQL * sql)
{
	_mysql = sql;
}

SqlPrepare * DBTask::prepare()
{
	return _pre;
}

void DBTask::complete_back(std::function<void()> backfunc)
{
	_complete_back = backfunc;
}

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

	m_db = new DBInterfaceMysql();
	m_db->connect(pool->host(), pool->dbname(), pool->user(), pool->pswd(), pool->port());
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
	static_cast<DBTask*>(task)->mysql(m_db->mysql());
	CThread::run(task);
}

DBThreadPool::DBThreadPool(const char * host, const char * dbname, const char * user, const char * pswd, unsigned int port)
{
	_host = host;
	_dbname = dbname;
	_user = user;
	_pswd = pswd;
	_port = port;
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
const char * DBThreadPool::host()
{
	return _host.c_str();
}
const char * DBThreadPool::dbname()
{
	return _dbname.c_str();
}
const char * DBThreadPool::user()
{
	return _user.c_str();
}
const char * DBThreadPool::pswd()
{
	return _pswd.c_str();
}
unsigned int DBThreadPool::port()
{
	return _port;
}
