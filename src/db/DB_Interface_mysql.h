#ifndef DB_INTERFACE_MYSQL_H
#define DB_INTERFACE_MYSQL_H

#include "DB_Interface.h"

class MysqlResult;
class SqlPrepare;
class DBInterfaceMysql : public DB_Interface
{
public:
	DBInterfaceMysql(const char * host, const char * dbname, const char * user, const char * pswd = "", unsigned int port = 3306);
	~DBInterfaceMysql();
	
	virtual bool connect();
	virtual bool detach();
	virtual int execute(DBResult * result, const char * cmd, int len = 0);

	virtual const char * getError();
	virtual int getErrno();
	virtual bool ping();

	MYSQL * mysql();

protected:
	MYSQL mMysql_;

	std::string m_dbname;
	std::string m_user;
	std::string m_pswd;
};

namespace MySQL
{
	int threadSafe();
	void libraryInit();
	void libraryEnd();
	char const* getLibraryVersion();
};

#endif