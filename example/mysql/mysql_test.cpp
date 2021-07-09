#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "mysql.h"
#include "DB_Interface_mysql.h"
#include "MysqlResult.h"
#include "SqlPrepare.h"


void sql_query_1()
{
	DBInterfaceMysql mysql("127.0.0.1", "jw_test", "root", "111111");
	mysql.connect();

	
	MysqlResult result;
	mysql.execute(&result, "select * from test");

	while (result.fetch())
	{

		int id;
		int num;
		std::string name;
		result >> id;
		result >> num;
		result >> name;

		printf("id:%d, num:%d, name:%s\n", id, num, name.c_str());
	}
}

#include "SqlResultSet.h"

void sql_query_2()
{
	DBInterfaceMysql mysql("127.0.0.1", "jw_test", "root", "111111");
	mysql.connect();


	SqlPrepare pre("select * from test where id = ?");
	pre.pushInt32(1);
	pre.prepare(mysql.mysql());

	SqlResultSet result;
	pre.execute(&result);

	while (result.fetch())
	{
		int id = result.getInt32();
		int num = result.getInt32();
		std::string name = result.getString();

		printf("id:%d, num:%d, name:%s\n", id, num, name.c_str());
	}
}

#include <functional>
#include "DBThreadPool.h"
void sql_query_3()
{
	DBConfig config;
	config.dbname = "jw_test";
	config.pswd = "111111";
	config.ip = "127.0.0.1";
	DBThreadPool pool(config);
	pool.create(1);

	{
		std::shared_ptr<SqlPrepare> pre(new SqlPrepare("select * from test where id = ?"));
		pre->pushInt32(1);

		std::shared_ptr<DBSqlTask> task(new DBSqlTask(pre, std::make_shared<SqlResultSet>()));
		task->backfunc = [](int errno_, const char* error, std::shared_ptr<SqlResultSet> result) {

			if (errno_ != 0)
			{
				printf("%s\n", error);
			}
			else
			{
				while (result->fetch())
				{
					int id = result->getInt32();
					int num = result->getInt32();
					std::string name = result->getString();
					printf("id:%d, num:%d, name:%s\n", id, num, name.c_str());
				}
			}
		};

		pool.addTask(task);
	}

	while (1)
	{
		Sleep(10);
		pool.update();
	}
}

#include <iostream>
int main()
{
	MySQL::libraryInit();
	MySQL::threadSafe();

	sql_query_1();
	std::cout << "**************************************" << std::endl;
	sql_query_2();
	std::cout << "**************************************" << std::endl;
	sql_query_3();

	MySQL::libraryEnd();

	return 0;
}