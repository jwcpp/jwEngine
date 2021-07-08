#include "sol/sol.hpp"

#include "mysql.h"
#include "DB_Interface_mysql.h"
#include "MysqlResult.h"
#include "SqlPrepare.h"
#include "DBThreadPool.h"
#include "SqlResultSet.h"

class Lua_SqlResult
{
public:
	Lua_SqlResult(std::shared_ptr<SqlResultSet> result):
		m_result(result)
	{
	}

	//get
	int8 getInt8() { return m_result->getInt8(); }
	uint8 getUint8() { return m_result->getUint8(); }
	int16 getInt16() { return m_result->getInt16(); }
	uint16 getUint16() { return m_result->getUint16(); }
	int32 getInt32() { return m_result->getInt32(); }
	uint32 getUint32() { return m_result->getUint32(); }
	int64 getInt64() { return m_result->getInt64(); }
	uint64 getUint64() { return m_result->getUint64(); }
	float getFloat() { return m_result->getFloat(); }
	double getDouble() { return m_result->getDouble(); }
	std::string getString() { return m_result->getString(); }
	int readBlob(BasePacket * packet) { return m_result->readBlob(packet); }
	std::string_view getData() { return m_result->getStrview(); } // lua call

	bool emptyField(int idx) { return m_result->emptyField(idx); }
	bool empty() { return m_result->isEmpty(); }

	bool fetch()
	{
		return m_result->fetch();
	}

private:
	std::shared_ptr<SqlResultSet> m_result;
};

class Lua_SqlCommand
{
public:
	Lua_SqlCommand(const char * sql):
		m_sqlPre(new SqlPrepare(sql))
	{

	}

	~Lua_SqlCommand()
	{
		
	}

	void addToPool(DBThreadPool * pool, std::function<void(int32, const char *, Lua_SqlResult *)> backfunc)
	{
		std::shared_ptr<DBSqlTask> dbTask(new DBSqlTask(m_sqlPre, std::make_shared<SqlResultSet>()));

		// back func
		dbTask->backfunc = [backfunc](int32 errno_, const char* err, std::shared_ptr<SqlResultSet> result) {

			if (backfunc != nullptr)
			{
				Lua_SqlResult _result(result);
				backfunc(errno_, err, &_result);
			}
		};
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
	void pushData(std::string_view sv) { m_sqlPre->pushData(sv); } // lua call

private:
	std::shared_ptr<SqlPrepare> m_sqlPre;
};



void luabind_mysql(sol::state & lua)
{
	{
		lua["mysql_thread_safe"] = &MySQL::threadSafe;
		lua["mysql_library_init"] = &MySQL::libraryInit;
		lua["mysql_library_end"] = &MySQL::libraryEnd;
		lua["mysql_version"] = &MySQL::getLibraryVersion;
	}

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
		"pushData", &Lua_SqlCommand::pushData,
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
		"getData", &Lua_SqlResult::getData,
		"fetch", &Lua_SqlResult::fetch,
		"emptyField", &Lua_SqlResult::emptyField,
		"empty", &Lua_SqlResult::empty);
}