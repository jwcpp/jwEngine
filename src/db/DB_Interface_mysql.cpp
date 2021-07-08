#include <string>
#include <cstring>
#include "mysql.h"
#include "DB_Interface_mysql.h"
#include "XLog.h"
#include "MysqlResult.h"
#include "SqlPrepare.h"

DBInterfaceMysql::DBInterfaceMysql(const char * host, const char * dbname, const char * user, const char * pswd, unsigned int port):
	DB_Interface(host, port)
{
	m_dbname = dbname;
	m_user = user;
	m_pswd = pswd;

	if (!mysql_init(&mMysql_))
	{
		ERROR_LOG("mysql init error!!");
	}
}

DBInterfaceMysql::~DBInterfaceMysql()
{
	detach();
}

bool DBInterfaceMysql::connect()
{
	if (mysql_options(&mMysql_, MYSQL_SET_CHARSET_NAME, "utf8"))
	{
		ERROR_LOG("mysql_options(MYSQL_SET_CHARSET_NAME) Errno:%d failed: %s", getErrno(), getError());
		return false;
	}

	my_bool reconnect = 1;
	if (mysql_options(&mMysql_, MYSQL_OPT_RECONNECT, &reconnect))
	{
		ERROR_LOG("mysql_options(MYSQL_OPT_RECONNECT) Errno:%d failed: %s", getErrno(), getError());
		return false;
	}

	if (!mysql_real_connect(&mMysql_, m_ip.c_str(), m_user.c_str(), m_pswd.c_str(), m_dbname.c_str(), m_port, NULL, 0))
	{
		ERROR_LOG("mysql_real_connect Errno:%d error: %s", getErrno(), getError());
		return false;
	}

	return true;
}

bool DBInterfaceMysql::detach()
{
	::mysql_close(&mMysql_);
	return true;
}

int DBInterfaceMysql::execute(DBResult * result, const char * cmd, int len)
{
	int nResult = mysql_real_query(&mMysql_, cmd, (len <= 0 ? strlen(cmd) : len));
	if (nResult != 0)
	{
		ERROR_LOG("mysql_real_query Errno:%d error: %s", getErrno(), getError());
		return -1;
	}

	MYSQL_RES * pResult = mysql_store_result(&mMysql_);

	/*
	if (pResult)
	{
	uint32 nrows = (uint32)mysql_num_rows(pResult);
	uint32 nfields = (uint32)mysql_num_fields(pResult);

	MYSQL_ROW arow;
	while ((arow = mysql_fetch_row(pResult)) != NULL)
	{
	unsigned long *lengths = mysql_fetch_lengths(pResult);

	for (uint32 i = 0; i < nfields; ++i)
	{
	// arow[i], lengths[i]
	}
	}

	//mysql_free_result(pResult);
	}
	else
	{

	}
	*/

	static_cast<MysqlResult *>(result)->setResult(pResult);

	return 0;
}

const char * DBInterfaceMysql::getError()
{
	return mysql_error(&mMysql_);
}

int DBInterfaceMysql::getErrno()
{
	return mysql_errno(&mMysql_);
}

bool DBInterfaceMysql::ping() {
	return mysql_ping(&mMysql_) == 0;
}

MYSQL * DBInterfaceMysql::mysql() {
	return &mMysql_;
}


namespace MySQL
{
	int threadSafe()
	{
		return mysql_thread_safe();
	}
	void libraryInit()
	{
		mysql_library_init(-1, nullptr, nullptr);
	}
	void libraryEnd()
	{
		mysql_library_end();
	}
	char const* getLibraryVersion()
	{
		return MYSQL_SERVER_VERSION;
	}
};