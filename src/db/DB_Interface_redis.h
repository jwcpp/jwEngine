#pragma once
/************************************************************************
* @file      DB_Interface_redis.h
* @brief     db interface redis
* @author    jiangwang
* @data      2020-3-31
* @version   0.1
************************************************************************/

#include "DB_Interface.h"
class RedisCommand;
class DBInterfaceRedis : public DB_Interface
{
public:
	DBInterfaceRedis(const char * ip, unsigned int port);
	~DBInterfaceRedis();

	virtual bool connect();
	virtual bool detach();
	virtual int execute(DBResult * result, const char * cmd, int len = 0);
	virtual const char * getError();
	virtual int getErrno();
	virtual bool ping();

	int execute(RedisCommand * command, DBResult * result);

protected:
	redisContext * m_context;
};

