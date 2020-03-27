#ifndef DB_INTERFACE_MYSQL_H
#define DB_INTERFACE_MYSQL_H

#include "mysql.h"

class DBResult;
class SqlPrepare;
class DBInterfaceMysql
{
public:
	DBInterfaceMysql();
	~DBInterfaceMysql();
	
	int connect(const char * host, const char * dbname, const char * user, const char * pswd = "", unsigned int port = 3306);
	int execute(DBResult * result, const char * cmd, int len = 0);

	const char * getError();
	int getErrno();
	bool ping();
	MYSQL * mysql();

private:
	std::string getErrorLog();
private:
	MYSQL mMysql_;
};

#endif