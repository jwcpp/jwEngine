#include <stdio.h>
#include <stdlib.h>

#include <string>
#include "hiredis.h"
#include "DB_Interface_redis.h"
#include "XLog.h"
#include "RedisResult.h"
#include <vector>
#include "RedisCommand.h"
#include <functional>
#include "DBThreadPool.h"
#include "Tools.h"

void select(DBThreadPool * pool)
{
	std::shared_ptr<RedisCommand> command(new RedisCommand("lrange"));
	command->pushString("t1");
	command->pushString("0");
	command->pushString("10");

	std::shared_ptr<RedisResult> result(new RedisResult);

	std::shared_ptr<DBRedisTask> task(new DBRedisTask(command, result));

	task->backfunc = [](const char * err, std::shared_ptr<RedisResult> result) {

		while (result->fetch())
		{
			int32 value;
			*result >> value;
			printf("%d\n", value);
		}
	};

	pool->addTask(task);
}

void insert(DBThreadPool * pool)
{
	std::shared_ptr<RedisCommand> command(new RedisCommand("lpush"));
	command->pushString("t1");
	command->pushInt32(1);
	command->pushInt32(22);
	command->pushInt32(333);

	std::shared_ptr<DBRedisTask> task(new DBRedisTask(command, std::make_shared<RedisResult>()));
	task->backfunc = [pool](const char* err, std::shared_ptr<RedisResult> result) {
		select(pool);
	};

	pool->addTask(task);
}

int main()
{
	DBConfig config;
	config.device = "redis";
	config.ip = "127.0.0.1";
	config.port = 6379;

	DBThreadPool pool(config);
	pool.create(1);

	insert(&pool);

	while (1)
	{
		Tools::sleep(10);
		pool.update();
	}

	system("pause");
	return 0;
}