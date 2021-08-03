#include "mysql.h"
#include "SqlResultSet.h"
#include "XLog.h"
#include "BasePacket.h"

static void allocateResultBuffer(MYSQL_BIND* bind, MYSQL_FIELD* field)
{
	if (field)
	{
		bind->buffer_type = field->type;
		bind->is_unsigned = field->flags & UNSIGNED_FLAG;
		switch (bind->buffer_type)
		{
		case MYSQL_TYPE_TINY:
			bind->buffer_length = 1;
			bind->buffer = new char[bind->buffer_length];
			break;
		case MYSQL_TYPE_YEAR:
		case MYSQL_TYPE_SHORT:
			bind->buffer_length = 2;
			bind->buffer = new char[bind->buffer_length];
			break;
		case MYSQL_TYPE_INT24:
		case MYSQL_TYPE_LONG:
			bind->buffer_length = 4;
			bind->buffer = new char[bind->buffer_length];
			break;
		case MYSQL_TYPE_LONGLONG:
			bind->buffer_length = 8;
			bind->buffer = new char[bind->buffer_length];
			break;
		case MYSQL_TYPE_FLOAT:
			bind->buffer_length = 4;
			bind->buffer = new char[bind->buffer_length];
			break;
		case MYSQL_TYPE_DOUBLE:
			bind->buffer_length = 8;
			bind->buffer = new char[bind->buffer_length];
			break;
		case MYSQL_TYPE_NEWDECIMAL:
			bind->buffer_length = 67;
			bind->buffer = new char[bind->buffer_length];
			break;

		case MYSQL_TYPE_TIME:
		case MYSQL_TYPE_DATE:
		case MYSQL_TYPE_DATETIME:
		case MYSQL_TYPE_TIMESTAMP:
			bind->buffer_length = sizeof(MYSQL_TIME);
			bind->buffer = new char[bind->buffer_length];
			break;
		case MYSQL_TYPE_STRING:
		case MYSQL_TYPE_VAR_STRING:
			bind->buffer_length = field->length;
			bind->buffer = new char[bind->buffer_length];
			break;
		case MYSQL_TYPE_TINY_BLOB:
		case MYSQL_TYPE_BLOB:
		case MYSQL_TYPE_MEDIUM_BLOB:
		case MYSQL_TYPE_LONG_BLOB:
			bind->buffer_length = field->max_length;
			bind->buffer = new char[bind->buffer_length];
			break;
		case MYSQL_TYPE_BIT:
			bind->buffer_length = 1;
			bind->buffer = new char[bind->buffer_length];
			break;
		default:
			ERROR_LOG("allocateResultBuffer() failed: Unknown type for the result.");
		}
	}
	else
	{
		bind->buffer_type = MYSQL_TYPE_NULL;
	}
}

SqlResultSet::SqlResultSet():
	m_currRowIdx(-1), m_currColIdx(0),m_rowCount(0),m_fieldCount(0)
{

}

SqlResultSet::~SqlResultSet()
{
	
}

void SqlResultSet::setResult(MYSQL_STMT* stmt, MYSQL_RES* pResult, uint64 pRowCount, uint32 pFieldCount)
{
	m_rowCount = pRowCount;
	m_fieldCount = pFieldCount;

	if (pRowCount == 0) return;

	MYSQL_BIND* resultBind = new MYSQL_BIND[pFieldCount];
	unsigned long* resultLengths = new unsigned long[pFieldCount];
	my_bool* isNulls = new my_bool[pFieldCount];
	memset(resultBind, 0, sizeof(MYSQL_BIND) * pFieldCount);

	uint32 nIndex = 0;
	MYSQL_FIELD* pField = NULL;
	while ((pField = mysql_fetch_field(pResult)))
	{
		allocateResultBuffer(&resultBind[nIndex], pField);
		resultBind[nIndex].length = &resultLengths[nIndex];
		resultBind[nIndex].is_null = &isNulls[nIndex];
		nIndex++;
	}

	if (!mysql_stmt_bind_result(stmt, resultBind))
	{
		m_fields.resize(uint32(pRowCount) * pFieldCount);

		// Ìî³ä×Ö¶ÎÊý¾Ý
		int rowIndex = 0;
		while (_fetch(stmt))
		{
			for (uint32 fIndex = 0; fIndex < pFieldCount; ++fIndex)
			{
				const char* pdata = NULL;
				int len = 0;
				if (!*resultBind[fIndex].is_null)
				{
					pdata = (const char*)resultBind[fIndex].buffer;
					len = *resultBind[fIndex].length;
				}

				m_fields[rowIndex * pFieldCount + fIndex].make(pdata, len);
			}
			rowIndex++;
		}
	}

	// delete
	for (int i = 0; i< pFieldCount; ++i)
	{
		MYSQL_BIND* pBind = &resultBind[i];
		if(pBind->buffer) delete [] pBind->buffer;
	}

	delete[] resultBind;
	delete[] resultLengths;
	delete[] isNulls;
}

bool SqlResultSet::fetch()
{
	if (m_currRowIdx + 1 >= m_rowCount)
	{
		return false;
	}
	m_currRowIdx++;
	m_currColIdx = 0;

	return true;
}

bool SqlResultSet::isEmpty()
{
	return m_rowCount == 0;
}
uint32 SqlResultSet::getRowCount()
{
	return m_rowCount;
}
uint32 SqlResultSet::getFieldsCount()
{
	return m_fieldCount;
}
const char* SqlResultSet::getData(int& len)
{
	SqlField* field = getField(m_currColIdx++);
	if (!field) return NULL;

	len = field->length();
	return field->data();
}
const char* SqlResultSet::getData()
{
	SqlField* field = getField(m_currColIdx++);
	if (!field) return NULL;
	return field->data();
}

bool SqlResultSet::_fetch(MYSQL_STMT* stmt)
{
	switch (mysql_stmt_fetch(stmt))
	{
	case 0:
	case MYSQL_DATA_TRUNCATED:
		return true;
	case MYSQL_NO_DATA:
		return false;

	case 1: // the error
	default:
		ERROR_LOG(mysql_stmt_error(stmt));
	}

	return false;
}

SqlField* SqlResultSet::getField(int idx)
{
	if (m_currRowIdx < 0 || m_currRowIdx >= m_rowCount || idx >= m_fieldCount)
	{
		return NULL;
	}

	SqlField* field = &m_fields[m_currRowIdx * m_fieldCount + idx];
	if (field->isEmpty())
	{
		return NULL;
	}

	return field;
}

bool SqlResultSet::emptyField(int idx)
{
	return getField(idx);
}

int8 SqlResultSet::getInt8()
{
	SqlField* field = getField(m_currColIdx++);
	if (!field) return 0;

	return field->getValue<int8>();
}
uint8 SqlResultSet::getUint8()
{
	SqlField* field = getField(m_currColIdx++);
	if (!field) return 0;

	return field->getValue<uint8>();
}
int16 SqlResultSet::getInt16()
{
	SqlField* field = getField(m_currColIdx++);
	if (!field) return 0;

	return field->getValue<int16>();
}
uint16 SqlResultSet::getUint16()
{
	SqlField* field = getField(m_currColIdx++);
	if (!field) return 0;

	return field->getValue<uint16>();
}
int32 SqlResultSet::getInt32()
{
	SqlField* field = getField(m_currColIdx++);
	if (!field) return 0;

	return field->getValue<int32>();
}
uint32 SqlResultSet::getUint32()
{
	SqlField* field = getField(m_currColIdx++);
	if (!field) return 0;

	return field->getValue<uint32>();
}
int64 SqlResultSet::getInt64()
{
	SqlField* field = getField(m_currColIdx++);
	if (!field) return 0;

	return field->getValue<int64>();
}
uint64 SqlResultSet::getUint64()
{
	SqlField* field = getField(m_currColIdx++);
	if (!field) return 0;

	return field->getValue<uint64>();
}
float SqlResultSet::getFloat()
{
	SqlField* field = getField(m_currColIdx++);
	if (!field) return 0.0f;

	return field->getValue<float>();
}
double SqlResultSet::getDouble()
{
	SqlField* field = getField(m_currColIdx++);
	if (!field) return 0.0f;

	return field->getValue<double>();
}
std::string SqlResultSet::getString()
{
	SqlField* field = getField(m_currColIdx++);
	if (!field) return "";

	std::string val(field->data(), field->length());
	return val;
}
int SqlResultSet::readBlob(BasePacket* packet)
{
	SqlField* field = getField(m_currColIdx++);
	if (!field) return 0;

	packet->append(field->data(), field->length());
	return field->length();
}

std::string_view SqlResultSet::getStrview() // lua call
{
	SqlField* field = getField(m_currColIdx++);
	if (!field) return "";

	std::string_view val(field->data(), field->length());
	return val;
}