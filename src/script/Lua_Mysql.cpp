#include "sol/sol.hpp"

#include "DB_Interface_mysql.h"
#include "DBResult.h"
#include "SqlPrepare.h"
#include "DBThreadPool.h"

class Lua_SqlResult
{
public:
	Lua_SqlResult(SqlPrepare * pre)
	{
		sqlPre = pre;
	}

	//get
	int8 getInt8() { return sqlPre->getInt8(); }
	uint8 getUint8() { return sqlPre->getUint8(); }
	int16 getInt16() { return sqlPre->getInt16(); }
	uint16 getUint16() { return sqlPre->getUint16(); }
	int32 getInt32() { return sqlPre->getInt32(); }
	uint32 getUint32() { return sqlPre->getUint32(); }
	int64 getInt64() { return sqlPre->getInt64(); }
	uint64 getUint64() { return sqlPre->getUint64(); }
	float getFloat() { return sqlPre->getFloat(); }
	double getDouble() { return sqlPre->getDouble(); }
	std::string getString() { return sqlPre->getString(); }

	bool fetch()
	{
		return sqlPre->fetch();
	}

	SqlPrepare * sqlPre = NULL;
	std::function<void(Lua_SqlResult *)> backfunc;
};

class Lua_SqlCommand
{
public:
	Lua_SqlCommand(const char * sql)
	{
		SqlPrepare * sqlPre = new SqlPrepare(sql);
		DBTask * dbTask = new DBTask(sqlPre);
		Lua_SqlResult * sqlRet = new Lua_SqlResult(sqlPre);

		// back func
		dbTask->complete_back(
			[sqlPre, dbTask, sqlRet](){

			if (sqlRet->backfunc != nullptr)
			{
				sqlRet->backfunc(sqlRet);
			}
			delete sqlPre;
			delete dbTask;
			delete sqlRet;
			}
		);

		_sqlPre = sqlPre;
		_dbTask = dbTask;
		_sqlRet = sqlRet;
	}

	~Lua_SqlCommand()
	{
		printf("ssss");
	}

	void setBackfunc(std::function<void(Lua_SqlResult *)> backfunc)
	{
		_sqlRet->backfunc = backfunc;
	}

	void addToPool(DBThreadPool * pool)
	{
		pool->addTask(_dbTask);
	}

	// push
	void pushInt8(int8 value) { _sqlPre->pushInt8(value); }
	void pushUint8(uint8 value) { _sqlPre->pushUint8(value); }
	void pushInt16(int16 value) { _sqlPre->pushInt16(value); }
	void pushUint16(uint16 value) { _sqlPre->pushUint16(value); }
	void pushInt32(int32 value) { _sqlPre->pushInt32(value); }
	void pushUint32(uint32 value) { _sqlPre->pushUint32(value); }
	void pushInt64(int64 value) { _sqlPre->pushInt64(value); }
	void pushUint64(uint64 value) { _sqlPre->pushUint64(value); }
	void pushFloat(float value) { _sqlPre->pushFloat(value); }
	void pushDouble(double value) { _sqlPre->pushDouble(value); }
	void pushString(std::string value) { _sqlPre->pushString(value); }

	DBTask * _dbTask = NULL;
	SqlPrepare * _sqlPre = NULL;
	Lua_SqlResult * _sqlRet = NULL;
};



void luabind_mysql(sol::state & lua)
{
	lua.new_usertype<DBThreadPool>("DBThreadPool",
		sol::constructors<DBThreadPool(const char *, const char *, const char *, const char *, unsigned int)>(),
		"create", &DBThreadPool::create,
		"exit", &DBThreadPool::exit,
		"update", &DBThreadPool::update);

	lua.new_usertype<Lua_SqlCommand>("SqlCommand",
		sol::constructors<DBThreadPool(const char *)>(),
		"pushInt8", &Lua_SqlCommand::pushInt8,
		"pushUint8", &Lua_SqlCommand::pushUint8,
		"pushInt16", &Lua_SqlCommand::pushInt16,
		"pushUint16", &Lua_SqlCommand::pushUint16,
		"pushInt32", &Lua_SqlCommand::pushInt32,
		"pushUint32", &Lua_SqlCommand::pushUint32,
		"pushInt64", &Lua_SqlCommand::pushInt64,
		"pushUint64", &Lua_SqlCommand::pushUint64,
		"pushFloat", &Lua_SqlCommand::pushFloat,
		"pushDouble", &Lua_SqlCommand::pushDouble,
		"pushString", &Lua_SqlCommand::pushString,
		"setBackfunc", &Lua_SqlCommand::setBackfunc,
		"addToPool", &Lua_SqlCommand::addToPool);

	lua.new_usertype<Lua_SqlResult>("SqlResult",
		"getInt8", &Lua_SqlResult::getInt8,
		"getUint8", &Lua_SqlResult::getUint8,
		"getInt16", &Lua_SqlResult::getInt16,
		"getUint16", &Lua_SqlResult::getUint16,
		"getInt32", &Lua_SqlResult::getInt32,
		"getUint32", &Lua_SqlResult::getUint32,
		"getInt64", &Lua_SqlResult::getInt64,
		"getUint64", &Lua_SqlResult::getUint64,
		"getFloat", &Lua_SqlResult::getFloat,
		"getDouble", &Lua_SqlResult::getDouble,
		"getString", &Lua_SqlResult::getString,
		"fetch", &Lua_SqlResult::fetch);
}