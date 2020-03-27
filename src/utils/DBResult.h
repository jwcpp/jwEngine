#ifndef DB_RESULT_H
#define DB_RESULT_H

#include "mysql.h"
#include "XLog.h"
#include "BaseType.h"
#include <strstream>

class DBResult
{
public:
	DBResult();
	~DBResult();

	void setResult(MYSQL_RES * result);

	bool isEmpty();
	bool fetch();
	uint32 getRowCount();
	uint32 getFieldsCount();
	
	const char * getData(int & len);
	const char * getData();

	template<typename T>
	DBResult & operator >> (T & t)
	{
		if (pos >= getFieldsCount())
		{
			ERROR_LOG("mysql row count upper limit");
			return *this;
		}
		std::istrstream ins(aRow[pos]);
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