#include "DBResult.h"

DBResult::DBResult()
{
	pResult = NULL;
	pos = 0;
	lengths = NULL;
}

DBResult::~DBResult()
{
	if (pResult)
	{
		mysql_free_result(pResult);
	}
}

void DBResult::setResult(MYSQL_RES * result) {
	pResult = result;
	if (result)
		lengths = mysql_fetch_lengths(pResult);
	else
		lengths = NULL;
}

bool DBResult::isEmpty()
{
	return pResult == NULL;
}

bool DBResult::fetch() {
	pos = 0;
	return (aRow = mysql_fetch_row(pResult)) != NULL;
}

uint32 DBResult::getRowCount() {
	return (uint32)mysql_num_rows(pResult);
}

uint32 DBResult::getFieldsCount() {
	return (uint32)mysql_num_fields(pResult);
}

const char * DBResult::getData(int & len) {
	len = lengths[pos];
	return aRow[pos++];
}

const char * DBResult::getData() {
	return aRow[pos++];
}