#ifndef DB_RESULT_H
#define DB_RESULT_H

#include "mysql.h"
#include "XLog.h"
#include "BaseType.h"
#include <sstream>
#include "DBResult.h"

class MysqlResult : public DBResult
{
public:
	MysqlResult();
	~MysqlResult();

	void setResult(MYSQL_RES * result);

	virtual bool isEmpty();
	virtual bool fetch();
	virtual uint32 getRowCount();
	virtual uint32 getFieldsCount();
	
	virtual const char * getData(int & len);
	virtual const char * getData();

	template<typename T>
	MysqlResult & operator >> (T & t)
	{
		if (pos >= getFieldsCount())
		{
			ERROR_LOG("mysql row count upper limit");
			return *this;
		}
		std::istringstream ins(aRow[pos]);
		ins >> t;
		pos++;
		return *this;
	}

private:

	MYSQL_RES * pResult;
	MYSQL_ROW aRow;
	uint32 pos;
	unsigned long *lengths;
};

#endif