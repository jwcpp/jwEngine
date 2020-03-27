#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "DB_Interface_mysql.h"
#include "DBResult.h"
#include "SqlPrepare.h"

int _main()
{
	DBInterfaceMysql mysql;
	mysql.connect("127.0.0.1", "jw_test", "root", "1111");

	
	DBResult result;
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
	DBInterfaceMysql mysql;
	mysql.connect("127.0.0.1", "jw_test", "root", "1111");


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


#include "DBThreadPool.h"
int main()
{
	DBThreadPool pool("127.0.0.1", "jw_test", "root", "1111");

	SqlPrepare * pre = new SqlPrepare("select * from test where id = ?");
	pre->pushInt32(1);

	DBTask * task = new DBTask(pre);
	task->complete_back([&pre, &task]() {
		while (pre->fetch())
		{
			int id = pre->getInt32();
			int num = pre->getInt32();
			std::string name = pre->getString();
			printf("id:%d, num:%d, name:%s\n", id, num, name.c_str());
		}

		delete pre;
		delete task;
	});

	pool.create(1);
	pool.addTask(task);

	while (1)
	{
		Sleep(10);
		pool.update();
	}

	system("pause");
	return 0;
}