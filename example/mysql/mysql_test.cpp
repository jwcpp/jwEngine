#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "mysql.h"
#include "DB_Interface_mysql.h"
#include "MysqlResult.h"
#include "SqlPrepare.h"


int _main()
{
	DBInterfaceMysql mysql("127.0.0.1", "jw_test", "root", "1111");
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
	}

	system("pause");
	return 0;
}


int __main()
{
	DBInterfaceMysql mysql("127.0.0.1", "jw_test", "root", "1111");
	mysql.connect();


	SqlPrepare pre("select * from test where id = ?");
	pre.pushInt32(1);
	pre.prepare(mysql.mysql());
	pre.execute();

	while (pre.fetch())
	{
		int id = pre.getInt32();
		int num = pre.getInt32();
		std::string name = pre.getString();

		printf("id:%d, num:%d, name:%s\n", id, num, name.c_str());
	}

	system("pause");
	return 0;
}

#include <functional>
#include "DBThreadPool.h"
int main()
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

		std::shared_ptr<DBSqlTask> task(new DBSqlTask(pre));
		task->backfunc = [](int errno_, const char* error, std::shared_ptr<SqlPrepare> pre) {

			if (errno_ != 0)
			{
				printf("%s\n", error);
			}
			else
			{
				while (pre->fetch())
				{
					int id = pre->getInt32();
					int num = pre->getInt32();
					std::string name = pre->getString();
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

	system("pause");
	return 0;
}