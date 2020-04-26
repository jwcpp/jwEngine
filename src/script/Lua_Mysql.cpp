#include "sol/sol.hpp"

#include "mysql.h"
#include "DB_Interface_mysql.h"
#include "MysqlResult.h"
#include "SqlPrepare.h"
#include "DBThreadPool.h"

class Lua_SqlResult
{
public:
	Lua_SqlResult(SqlPrepare * pre)
	{
		m_sqlPre = pre;
	}

	//get
	int8 getInt8() { return m_sqlPre->getInt8(); }
	uint8 getUint8() { return m_sqlPre->getUint8(); }
	int16 getInt16() { return m_sqlPre->getInt16(); }
	uint16 getUint16() { return m_sqlPre->getUint16(); }
	int32 getInt32() { return m_sqlPre->getInt32(); }
	uint32 getUint32() { return m_sqlPre->getUint32(); }
	int64 getInt64() { return m_sqlPre->getInt64(); }
	uint64 getUint64() { return m_sqlPre->getUint64(); }
	float getFloat() { return m_sqlPre->getFloat(); }
	double getDouble() { return m_sqlPre->getDouble(); }
	std::string getString() { return m_sqlPre->getString(); }
	int readBlob(BasePacket * packet) { return m_sqlPre->readBlob(packet); }

	bool fetch()
	{
		return m_sqlPre->fetch();
	}

private:
	SqlPrepare * m_sqlPre;
};

class Lua_SqlCommand
{
public:
	Lua_SqlCommand(const char * sql)
	{
		m_sqlPre = new SqlPrepare(sql);
	}

	~Lua_SqlCommand()
	{
		
	}

	void addToPool(DBThreadPool * pool, std::function<void(Lua_SqlResult *)> backfunc)
	{
		SqlPrepare * sqlPre = m_sqlPre;
		DBSqlTask * dbTask = new DBSqlTask(sqlPre);

		// back func
		dbTask->complete_back(
			[sqlPre, dbTask, backfunc]() {

			if (backfunc != nullptr)
			{
				Lua_SqlResult _result(sqlPre);
				backfunc(&_result);
			}
			delete sqlPre;
			delete dbTask;
		}
		);
		pool->addTask(dbTask);
	}

	// push
	void pushInt8(int8 value) { m_sqlPre->pushInt8(value); }
	void pushUint8(uint8 value) { m_sqlPre->pushUint8(value); }
	void pushInt16(int16 value) { m_sqlPre->pushInt16(value); }
	void pushUint16(uint16 value) { m_sqlPre->pushUint16(value); }
	void pushInt32(int32 value) { m_sqlPre->pushInt32(value); }
	void pushUint32(uint32 value) { m_sqlPre->pushUint32(value); }
	void pushInt64(int64 value) { m_sqlPre->pushInt64(value); }
	void pushUint64(uint64 value) { m_sqlPre->pushUint64(value); }
	void pushFloat(float value) { m_sqlPre->pushFloat(value); }
	void pushDouble(double value) { m_sqlPre->pushDouble(value); }
	void pushString(std::string value) { m_sqlPre->pushString(value); }
	void pushBlob(BasePacket * packet) { m_sqlPre->pushBlob(packet); }

private:
	SqlPrepare * m_sqlPre;
};



void luabind_mysql(sol::state & lua)
{
	lua.new_usertype<DBConfig>("DBConfig",
		"device", &DBConfig::device,
		"ip", &DBConfig::ip,
		"port", &DBConfig::port,
		"dbname", &DBConfig::dbname,
		"user", &DBConfig::user,
		"pswd", &DBConfig::pswd);

	lua.new_usertype<DBThreadPool>("DBThreadPool",
		sol::constructors<DBThreadPool(DBConfig)>(),
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
		"pushBlob", &Lua_SqlCommand::pushBlob,
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
		"readBlob", &Lua_SqlResult::readBlob,
		"fetch", &Lua_SqlResult::fetch);
}