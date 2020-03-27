#include <string>

#include "DB_Interface_mysql.h"
#include "XLog.h"
#include "DBResult.h"
#include "SqlPrepare.h"

DBInterfaceMysql::DBInterfaceMysql() {
	if (!mysql_init(&mMysql_))
	{
		ERROR_LOG("mysql init error!!");
	}
}

DBInterfaceMysql::~DBInterfaceMysql()
{
	mysql_close(&mMysql_);
}

int DBInterfaceMysql::connect(const char * host, const char * dbname, const char * user, const char * pswd, unsigned int port)
{
	if (mysql_options(&mMysql_, MYSQL_SET_CHARSET_NAME, "utf8"))
	{
		ERROR_LOG("mysql_options(MYSQL_SET_CHARSET_NAME) failed: %s", getError());
		return -1;
	}

	my_bool reconnect = 1;
	if (mysql_options(&mMysql_, MYSQL_OPT_RECONNECT, &reconnect))
	{
		ERROR_LOG("mysql_options(MYSQL_OPT_RECONNECT) failed: %s", getError());
		return -1;
	}

	if (!mysql_real_connect(&mMysql_, host, user, pswd, dbname, port, NULL, 0))
	{
		ERROR_LOG("mysql_real_connect error: %s", getError());
		return -1;
	}

	return 0;
}

int DBInterfaceMysql::execute(DBResult * result, const char * cmd, int len)
{
	int nResult = mysql_real_query(&mMysql_, cmd, (len <= 0 ? strlen(cmd) : len));
	if (nResult != 0)
	{
		ERROR_LOG("mysql_real_query error: %s", getError());
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

	result->setResult(pResult);

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

std::string DBInterfaceMysql::getErrorLog()
{
	std::string info;
	info.append("ErrorNo:" + std::to_string(mysql_errno(&mMysql_)) + ", ErrorMsg:");
	info.append(mysql_error(&mMysql_));

	return info;
}