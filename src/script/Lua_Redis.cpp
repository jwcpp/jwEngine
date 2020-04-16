#include "sol/sol.hpp"

#include "hiredis.h"
#include "DB_Interface_redis.h"
#include "RedisResult.h"
#include "DBThreadPool.h"
#include "RedisCommand.h"


class Lua_RedisResult
{
public:
	Lua_RedisResult()
	{
		
	}

	//get
	int8 getInt8() { return getValue<int8>(); }
	uint8 getUint8() { return getValue<uint8>(); }
	int16 getInt16() { return getValue<int16>(); }
	uint16 getUint16() { return getValue<uint16>(); }
	int32 getInt32() { return getValue<int32>(); }
	uint32 getUint32() { return getValue<uint32>(); }
	int64 getInt64() { return getValue<int64>(); }
	uint64 getUint64() { return getValue<uint64>(); }
	float getFloat() { return getValue<float>(); }
	double getDouble() { return getValue<double>(); }
	std::string getString() { return getValue<std::string>(); }
	int readBlob(BasePacket * packet) { return result.readBlob(packet); }

	bool fetch() {
		return result.fetch();
	}

	template<class T>
	T getValue()
	{
		T t;
		result >> t;
		return t;
	}

	RedisResult result;
	std::function<void(Lua_RedisResult *)> backfunc;
};

class Lua_RedisCommand
{
public:
	Lua_RedisCommand(const char * cmd)
	{
		RedisCommand *command = new RedisCommand(cmd);
		Lua_RedisResult *result = new Lua_RedisResult();
		DBRedisTask *dbTask = new DBRedisTask(command, &(result->result));

		// back func
		dbTask->complete_back(
			[command, dbTask, result]() {

			if (result->backfunc != nullptr)
			{
				result->backfunc(result);
			}
			delete command;
			delete dbTask;
			delete result;
		}
		);

		_command = command;
		_result = result;
		_dbTask = dbTask;
	}

	void pushInt8(int8 value) { _command->pushInt8(value); }
	void pushUint8(uint8 value) { _command->pushUint8(value); }
	void pushInt16(int16 value) { _command->pushInt16(value); }
	void pushUint16(uint16 value) { _command->pushUint16(value); }
	void pushInt32(int32 value) { _command->pushInt32(value); }
	void pushUint32(uint32 value) { _command->pushUint32(value); }
	void pushInt64(int64 value) { _command->pushInt64(value); }
	void pushUint64(uint64 value) { _command->pushUint64(value); }
	void pushFloat(float value) { _command->pushFloat(value); }
	void pushDouble(double value) { _command->pushDouble(value); }
	void pushString(std::string value) { _command->pushString(value); }
	void pushBlob(BasePacket * pack) { _command->pushBlob(pack); }

	void addToPool(DBThreadPool * pool)
	{
		pool->addTask(_dbTask);
	}

	void setBackfunc(std::function<void(Lua_RedisResult *)> backfunc)
	{
		_result->backfunc = backfunc;
	}

private:
	RedisCommand * _command;
	DBRedisTask * _dbTask;
	Lua_RedisResult * _result;
};

void luabind_redis(sol::state & lua)
{
	lua.new_usertype<Lua_RedisCommand>("RedisCommand",
		sol::constructors<Lua_RedisCommand(const char *)>(),
		"pushInt8", &Lua_RedisCommand::pushInt8,
		"pushUint8", &Lua_RedisCommand::pushUint8,
		"pushInt16", &Lua_RedisCommand::pushInt16,
		"pushUint16", &Lua_RedisCommand::pushUint16,
		"pushInt32", &Lua_RedisCommand::pushInt32,
		"pushUint32", &Lua_RedisCommand::pushUint32,
		"pushInt64", &Lua_RedisCommand::pushInt64,
		"pushUint64", &Lua_RedisCommand::pushUint64,
		"pushFloat", &Lua_RedisCommand::pushFloat,
		"pushDouble", &Lua_RedisCommand::pushDouble,
		"pushString", &Lua_RedisCommand::pushString,
		"pushBlob", &Lua_RedisCommand::pushBlob,
		"setBackfunc", &Lua_RedisCommand::setBackfunc,
		"addToPool", &Lua_RedisCommand::addToPool);

	lua.new_usertype<Lua_RedisResult>("RedisResult",
		"getInt8", &Lua_RedisResult::getInt8,
		"getUint8", &Lua_RedisResult::getUint8,
		"getInt16", &Lua_RedisResult::getInt16,
		"getUint16", &Lua_RedisResult::getUint16,
		"getInt32", &Lua_RedisResult::getInt32,
		"getUint32", &Lua_RedisResult::getUint32,
		"getInt64", &Lua_RedisResult::getInt64,
		"getUint64", &Lua_RedisResult::getUint64,
		"getFloat", &Lua_RedisResult::getFloat,
		"getDouble", &Lua_RedisResult::getDouble,
		"getString", &Lua_RedisResult::getString,
		"readBlob", &Lua_RedisResult::readBlob,
		"fetch", &Lua_RedisResult::fetch);
}