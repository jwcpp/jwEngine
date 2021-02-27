#include "BaseType.h"
#include <vector>
#include <string>
#include "RedisCommand.h"
#include "BasePacket.h"

RedisCommand::RedisCommand(const char *cmd)
{
	append((const uint8*)cmd, strlen(cmd));
}
void RedisCommand::pushInt8(int8 value)
{
	pushValue(value);
}
void RedisCommand::pushUint8(uint8 value)
{
	pushValue(value);
}
void RedisCommand::pushInt16(int16 value)
{
	pushValue(value);
}
void RedisCommand::pushUint16(uint16 value)
{
	pushValue(value);
}
void RedisCommand::pushInt32(int32 value)
{
	pushValue(value);
}
void RedisCommand::pushUint32(uint32 value)
{
	pushValue(value);
}
void RedisCommand::pushInt64(int64 value)
{
	pushValue(value);
}
void RedisCommand::pushUint64(uint64 value)
{
	pushValue(value);
}
void RedisCommand::pushFloat(float value)
{
	pushValue(value);
}
void RedisCommand::pushDouble(double value)
{
	pushValue(value);
}
void RedisCommand::pushString(std::string value)
{
	append((const uint8*)value.c_str(), value.size());
}
void RedisCommand::pushBlob(BasePacket * packet)
{
	append((const uint8*)packet->contents(), packet->writePos());
}

void RedisCommand::pushData(std::string_view sv)
{
	append((const uint8*)sv.data(), sv.size());
}

void RedisCommand::append(const uint8 * p, int len)
{
	std::vector<uint8> vec;
	vec.insert(vec.begin(), p, p + len);

	m_param.push_back(vec);
	m_len.push_back(len);
}

int RedisCommand::length()
{
	return m_len.size();
}
const char ** RedisCommand::argv(std::vector<char *> & vec)
{
	for (uint32 i = 0; i < m_param.size(); ++i)
	{
		vec.push_back((char *)(m_param[i].data()));
	}

	return (const char **)(vec.data());
}
const size_t * RedisCommand::argvlen()
{
	return (size_t *)m_len.data();
}

const char * RedisCommand::tostr()
{
	m_param[0].push_back(0);
	const char * ret = (const char *)(m_param[0].data());
	return ret;
}