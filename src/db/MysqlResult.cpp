#include "MysqlResult.h"

MysqlResult::MysqlResult()
{
	pResult = NULL;
	pos = 0;
	lengths = NULL;
}

MysqlResult::~MysqlResult()
{
	if (pResult)
	{
		mysql_free_result(pResult);
	}
}

void MysqlResult::setResult(MYSQL_RES * result) {
	pResult = result;
	if (result)
		lengths = mysql_fetch_lengths(pResult);
	else
		lengths = NULL;
}

bool MysqlResult::isEmpty()
{
	return pResult == NULL;
}

bool MysqlResult::fetch() {
	pos = 0;
	return (aRow = mysql_fetch_row(pResult)) != NULL;
}

uint32 MysqlResult::getRowCount() {
	return (uint32)mysql_num_rows(pResult);
}

uint32 MysqlResult::getFieldsCount() {
	return (uint32)mysql_num_fields(pResult);
}

const char * MysqlResult::getData(int & len) {
	len = lengths[pos];
	return aRow[pos++];
}

const char * MysqlResult::getData() {
	return aRow[pos++];
}