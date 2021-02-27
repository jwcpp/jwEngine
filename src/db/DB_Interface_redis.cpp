#include <string>
#include "hiredis.h"
#include "DB_Interface_redis.h"
#include "XLog.h"
#include "RedisResult.h"
#include <vector>
#include "RedisCommand.h"

DBInterfaceRedis::DBInterfaceRedis(const char * ip, unsigned int port):
	DB_Interface(ip, port)
{
	m_context = NULL;
}

DBInterfaceRedis::~DBInterfaceRedis()
{
	detach();
}

bool DBInterfaceRedis::connect()
{
	redisContext* c = redisConnect(m_ip.c_str(), (int)m_port);
	if (c->err)
	{
		ERROR_LOG("DBInterfaceRedis::attach: errno=%d, error=%s\n", c->err, c->errstr);

		redisFree(c);
		return false;
	}

	// TODO command: auth password
	// TODO command: select idx

	m_context = c;
	return true;
}

bool DBInterfaceRedis::detach()
{
	if (m_context)
	{
		redisFree(m_context);
		m_context = NULL;
	}

	return true;
}

int DBInterfaceRedis::execute(DBResult * result, const char * cmd, int len)
{
	redisReply *pRedisReply = (redisReply*)redisCommand(m_context, cmd);
	if (m_context->err)
	{
		ERROR_LOG("DBInterfaceRedis::query: cmd=%s, errno=%d, error=%s\n", cmd, m_context->err, m_context->errstr);
		return -1;
	}
	//freeReplyObject(pRedisReply);

	static_cast<RedisResult *>(result)->setResult(pRedisReply);
	return 0;
}

const char * DBInterfaceRedis::getError()
{
	if (m_context == NULL)
		return "pRedisContext_ is NULL";

	return m_context->errstr;
}

int DBInterfaceRedis::getErrno()
{
	if (m_context == NULL)
		return 0;

	return m_context->err;
}

bool DBInterfaceRedis::ping()
{
	if (!m_context)
		return false;

	redisReply* pRedisReply = (redisReply*)redisCommand(m_context, "ping");

	if (NULL == pRedisReply)
	{
		ERROR_LOG("DBInterfaceRedis::ping: errno=%d, error=%s\n",
			m_context->err, m_context->errstr);

		return false;
	}

	if (!(pRedisReply->type == REDIS_REPLY_STATUS && strcmp(pRedisReply->str, "PONG") == 0))
	{
		ERROR_LOG("DBInterfaceRedis::ping: errno=%d, error=%s\n",
			m_context->err, pRedisReply->str);

		freeReplyObject(pRedisReply);
		return false;
	}

	freeReplyObject(pRedisReply);
	return true;
}

int DBInterfaceRedis::execute(RedisCommand * command, DBResult * result)
{
	std::vector<char *> temp;
	redisReply *pRedisReply = NULL;
	
	if (command->length() > 1)
	{
		pRedisReply = (redisReply*)redisCommandArgv(m_context, command->length(), command->argv(temp), command->argvlen());
	}
	else
	{
		pRedisReply = (redisReply*)redisCommand(m_context, command->tostr());
	}

	if (m_context->err)
	{
		return -1;
	}
	//freeReplyObject(pRedisReply);

	static_cast<RedisResult *>(result)->setResult(pRedisReply);
	return 0;
}