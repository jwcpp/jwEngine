#include "BaseType.h"
#include <string>
#include "mysql.h"

#include "SqlPrepare.h"
#include "Tools.h"
#include "XLog.h"
#include "BasePacket.h"
#include "SpinLock.h"
#include <assert.h>

SpinLock sql_stmt_lock;

SqlPrepare::SqlPrepare(const char * sqlstr)
{
	m_stmt = NULL;
	m_paramBind = NULL;
	m_resultBind = NULL;
	m_field = 0;
	m_ridx = 0;
	m_widx = 0;

	m_sql = sqlstr;
	m_count = Tools::charCount((char *)sqlstr, '?');
	if (m_count > 0)
	{
		m_paramBind = new MYSQL_BIND[m_count];
		memset(m_paramBind, 0, sizeof(MYSQL_BIND) * m_count);
		m_paramLengths = new unsigned long[m_count];
		for (int i = 0; i < m_count; ++i)
		{
			m_paramBind[i].length = &m_paramLengths[i];
		}
	}
}

SqlPrepare::~SqlPrepare()
{
	sql_stmt_lock.lock();
	if (mysql_stmt_close(m_stmt))
	{
		ERROR_LOG(mysql_stmt_error(m_stmt));
	}
	sql_stmt_lock.unlock();

	if (m_paramBind != NULL)
	{
		for (int i = 0; i < m_count; ++i)
		{
			MYSQL_BIND* pBind = &m_paramBind[i];
			free(pBind->buffer);
		}

		delete[]m_paramBind;
		delete[]m_paramLengths;
		m_paramBind = NULL;
		m_paramLengths = NULL;
	}

	if (m_resultBind != NULL)
	{
		for (int i = 0; i < m_field; ++i)
		{
			MYSQL_BIND* pBind = &m_resultBind[i];
			free(pBind->buffer);
		}

		delete[]m_resultBind;
		delete[]m_resultLengths;
		delete[]m_isNulls;
		m_resultBind = NULL;
		m_resultLengths = NULL;
		m_isNulls = NULL;
	}
}

void SqlPrepare::clear()
{
	m_ridx = 0;
	m_widx = 0;
}

void SqlPrepare::pushInt8(int8 value)
{
	MYSQL_BIND* pBind = &m_paramBind[m_widx++];
	pBind->buffer_length = *pBind->length = sizeof(value);
	pBind->buffer_type = MYSQL_TYPE_TINY;
	pBind->is_unsigned = 0;

	if (pBind->buffer == NULL)
		allocateParamBuffer(pBind);
	*static_cast<int8*>(pBind->buffer) = value;
}

void SqlPrepare::pushUint8(uint8 value)
{
	MYSQL_BIND* pBind = &m_paramBind[m_widx++];
	pBind->buffer_length = *pBind->length = sizeof(value);
	pBind->buffer_type = MYSQL_TYPE_TINY;
	pBind->is_unsigned = UNSIGNED_FLAG;

	if (pBind->buffer == NULL)
		allocateParamBuffer(pBind);
	*static_cast<uint8*>(pBind->buffer) = value;
}
void SqlPrepare::pushInt16(int16 value)
{
	MYSQL_BIND* pBind = &m_paramBind[m_widx++];
	pBind->buffer_length = *pBind->length = sizeof(value);
	pBind->buffer_type = MYSQL_TYPE_SHORT;
	pBind->is_unsigned = 0;

	if (pBind->buffer == NULL)
		allocateParamBuffer(pBind);
	*static_cast<int16*>(pBind->buffer) = value;
}
void SqlPrepare::pushUint16(uint16 value)
{
	MYSQL_BIND* pBind = &m_paramBind[m_widx++];
	pBind->buffer_length = *pBind->length = sizeof(value);
	pBind->buffer_type = MYSQL_TYPE_SHORT;
	pBind->is_unsigned = UNSIGNED_FLAG;

	if (pBind->buffer == NULL)
		allocateParamBuffer(pBind);
	*static_cast<uint16*>(pBind->buffer) = value;
}
void SqlPrepare::pushInt32(int32 value)
{
	MYSQL_BIND* pBind = &m_paramBind[m_widx++];
	pBind->buffer_length = *pBind->length = sizeof(value);
	pBind->buffer_type = MYSQL_TYPE_LONG;
	pBind->is_unsigned = 0;

	if (pBind->buffer == NULL)
		allocateParamBuffer(pBind);
	*static_cast<int32*>(pBind->buffer) = value;
}
void SqlPrepare::pushUint32(uint32 value)
{
	MYSQL_BIND* pBind = &m_paramBind[m_widx++];
	pBind->buffer_length = *pBind->length = sizeof(value);
	pBind->buffer_type = MYSQL_TYPE_LONG;
	pBind->is_unsigned = UNSIGNED_FLAG;

	if (pBind->buffer == NULL)
		allocateParamBuffer(pBind);
	*static_cast<uint32*>(pBind->buffer) = value;
}
void SqlPrepare::pushInt64(int64 value)
{
	MYSQL_BIND* pBind = &m_paramBind[m_widx++];
	pBind->buffer_length = *pBind->length = sizeof(value);
	pBind->buffer_type = MYSQL_TYPE_LONGLONG;
	pBind->is_unsigned = 0;

	if (pBind->buffer == NULL)
		allocateParamBuffer(pBind);
	*static_cast<int64*>(pBind->buffer) = value;
}
void SqlPrepare::pushUint64(uint64 value)
{
	MYSQL_BIND* pBind = &m_paramBind[m_widx++];
	pBind->buffer_length = *pBind->length = sizeof(value);
	pBind->buffer_type = MYSQL_TYPE_LONGLONG;
	pBind->is_unsigned = UNSIGNED_FLAG;

	if (pBind->buffer == NULL)
		allocateParamBuffer(pBind);
	*static_cast<uint64*>(pBind->buffer) = value;
}
void SqlPrepare::pushFloat(float value)
{
	MYSQL_BIND* pBind = &m_paramBind[m_widx++];
	pBind->buffer_length = *pBind->length = sizeof(value);
	pBind->buffer_type = MYSQL_TYPE_FLOAT;
	pBind->is_unsigned = 0;

	if (pBind->buffer == NULL)
		allocateParamBuffer(pBind);
	*static_cast<float*>(pBind->buffer) = value;
}
void SqlPrepare::pushDouble(double value)
{
	MYSQL_BIND* pBind = &m_paramBind[m_widx++];
	pBind->buffer_length = *pBind->length = sizeof(value);
	pBind->buffer_type = MYSQL_TYPE_DOUBLE;
	pBind->is_unsigned = 0;

	if (pBind->buffer == NULL)
		allocateParamBuffer(pBind);
	*static_cast<double*>(pBind->buffer) = value;
}
void SqlPrepare::pushString(std::string value)
{
	MYSQL_BIND* pBind = &m_paramBind[m_widx++];

	int len = value.length();

	pBind->buffer_length = *pBind->length = len;
	pBind->buffer_type = MYSQL_TYPE_STRING;
	pBind->is_unsigned = 0;

	if (pBind->buffer == NULL)
		allocateParamBuffer(pBind);
	memcpy(pBind->buffer, value.data(), len);
}

void SqlPrepare::pushBlob(BasePacket * packet)
{
	write(packet->contents(), packet->writePos());
}

void SqlPrepare::pushData(std::string_view sv)
{
	write((void *)sv.data(), sv.size());
}

void SqlPrepare::write(void* pData, int len)
{
	MYSQL_BIND* pBind = &m_paramBind[m_widx++];

	pBind->buffer_length = *pBind->length = len;
	pBind->buffer_type = MYSQL_TYPE_BLOB;
	pBind->is_unsigned = 0;

	if (pBind->buffer == NULL)
		allocateParamBuffer(pBind);
	memcpy(pBind->buffer, pData, len);
}

int8 SqlPrepare::getInt8()
{
	MYSQL_BIND* pBind = &m_resultBind[m_ridx++];
	if (*pBind->is_null)
	{
		return 0;
	}

	return *static_cast<int8*>(pBind->buffer);
}

uint8 SqlPrepare::getUint8()
{
	MYSQL_BIND* pBind = &m_resultBind[m_ridx++];
	if (*pBind->is_null)
	{
		return 0;
	}

	return *static_cast<uint8*>(pBind->buffer);
}
int16 SqlPrepare::getInt16()
{
	MYSQL_BIND* pBind = &m_resultBind[m_ridx++];
	if (*pBind->is_null)
	{
		return 0;
	}

	return *static_cast<int16*>(pBind->buffer);
}
uint16 SqlPrepare::getUint16()
{
	MYSQL_BIND* pBind = &m_resultBind[m_ridx++];
	if (*pBind->is_null)
	{
		return 0;
	}

	return *static_cast<uint16*>(pBind->buffer);
}

int32 SqlPrepare::getInt32()
{
	MYSQL_BIND* pBind = &m_resultBind[m_ridx++];
	if (*pBind->is_null)
	{
		return 0;
	}

	return *static_cast<int32*>(pBind->buffer);
}
uint32 SqlPrepare::getUint32()
{
	MYSQL_BIND* pBind = &m_resultBind[m_ridx++];
	if (*pBind->is_null)
	{
		return 0;
	}

	return *static_cast<uint32*>(pBind->buffer);
}
int64 SqlPrepare::getInt64()
{
	MYSQL_BIND* pBind = &m_resultBind[m_ridx++];
	if (*pBind->is_null)
	{
		return 0;
	}

	return *static_cast<int64*>(pBind->buffer);
}
uint64 SqlPrepare::getUint64()
{
	MYSQL_BIND* pBind = &m_resultBind[m_ridx++];
	if (*pBind->is_null)
	{
		return 0;
	}

	return *static_cast<uint64*>(pBind->buffer);
}

float SqlPrepare::getFloat()
{
	MYSQL_BIND* pBind = &m_resultBind[m_ridx++];
	if (*pBind->is_null)
	{
		return 0;
	}

	return *static_cast<float*>(pBind->buffer);
}
double SqlPrepare::getDouble()
{
	MYSQL_BIND* pBind = &m_resultBind[m_ridx++];
	if (*pBind->is_null)
	{
		return 0;
	}

	return *static_cast<double*>(pBind->buffer);
}
std::string SqlPrepare::getString()
{
	MYSQL_BIND* pBind = &m_resultBind[m_ridx++];
	if (*pBind->is_null)
	{
		return "";
	}

	std::string str;
	str.assign(static_cast<const char*>(pBind->buffer), *pBind->length);
	return str;
}

int SqlPrepare::readBlob(BasePacket * packet)
{
	packet->resize(fieldLen());
	return read(packet->contents());
}

std::string SqlPrepare::getData()
{
	std::string retstr;
	int len = fieldLen();
	if (len > 0)
	{
		retstr.resize(len);
		read(retstr.data());
	}
	return retstr;
}

int SqlPrepare::fieldLen()
{
	MYSQL_BIND* pBind = &m_resultBind[m_ridx];
	if (*pBind->is_null)
	{
		return 0;
	}
	return *pBind->length;
}

int SqlPrepare::read(void * pData)
{
	MYSQL_BIND* pBind = &m_resultBind[m_ridx];
	if (*pBind->is_null)
	{
		return 0;
	}

	void* tmp = pBind->buffer;
	pBind->buffer = pData;
	pBind->buffer_length = *pBind->length;
	if (mysql_stmt_fetch_column(m_stmt, pBind, m_ridx++, 0))
	{
		pBind->buffer = tmp;
		pBind->buffer_length = 1;
		ERROR_LOG("mysql_stmt_fetch_column() failed: %s", mysql_stmt_error(m_stmt));
		return 0;
	}
	pBind->buffer = tmp;
	pBind->buffer_length = 1;

	return *pBind->length;
}

void SqlPrepare::allocateParamBuffer(MYSQL_BIND* bind)
{
	switch (bind->buffer_type)
	{
	case MYSQL_TYPE_TINY:
		bind->buffer = new char[1];
		break;
	case MYSQL_TYPE_SHORT:
		bind->buffer = new char[2];
		break;
	case MYSQL_TYPE_LONG:
		bind->buffer = new char[4];
		break;
	case MYSQL_TYPE_LONGLONG:
		bind->buffer = new char[8];
		break;
	case MYSQL_TYPE_FLOAT:
		bind->buffer = new char[4];
		break;
	case MYSQL_TYPE_DOUBLE:
		bind->buffer = new char[8];
		break;
	case MYSQL_TYPE_TIME:
	case MYSQL_TYPE_DATE:
	case MYSQL_TYPE_DATETIME:
	case MYSQL_TYPE_TIMESTAMP:
		bind->buffer = new char[sizeof(MYSQL_TIME)];
		break;
	case MYSQL_TYPE_STRING:
	case MYSQL_TYPE_BLOB:
		bind->buffer = new char[bind->buffer_length];
		break;
	case MYSQL_TYPE_NULL:
		break;
	default:
		ERROR_LOG("allocateParamBuffer() failed: Unknown type for the param.");
	}
}

void SqlPrepare::allocateResultBuffer(MYSQL_BIND* bind, MYSQL_FIELD *field)
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
			bind->buffer_length = 1;
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

int SqlPrepare::prepare(MYSQL * mysql)
{
	sql_stmt_lock.lock();
	m_stmt = mysql_stmt_init(mysql);
	if (!m_stmt)
	{
		sql_stmt_lock.unlock();
		return -1;
	}

	if (mysql_stmt_prepare(m_stmt, m_sql.data(), static_cast<unsigned long>(m_sql.length())))
	{
		sql_stmt_lock.unlock();
		return -1;
	}
	sql_stmt_lock.unlock();

	if (mysql_stmt_param_count(m_stmt) != m_count)
	{
		ERROR_LOG("mysql_stmt_param_count(m_stmt) != m_count");
		assert(0);
	}

	m_field = mysql_stmt_field_count(m_stmt);
	if (m_field)
	{
		m_resultBind = new MYSQL_BIND[m_field];
		m_resultLengths = new unsigned long[m_field];
		m_isNulls = new my_bool[m_field];
		memset(m_resultBind, 0, sizeof(MYSQL_BIND) * m_field);

		MYSQL_RES * resultMetadata = mysql_stmt_result_metadata(m_stmt);
		if (!resultMetadata)
		{
			return -1;
		}

		int nIndex = 0;
		MYSQL_FIELD* pField = NULL;
		while ((pField = mysql_fetch_field(resultMetadata)))
		{
			allocateResultBuffer(&m_resultBind[nIndex], pField);
			m_resultBind[nIndex].length = &m_resultLengths[nIndex];
			m_resultBind[nIndex].is_null = &m_isNulls[nIndex];
			nIndex++;
		}

		mysql_free_result(resultMetadata);

		if (mysql_stmt_bind_result(m_stmt, m_resultBind))
		{
			return -1;
		}
	}

	return 0;
}
int SqlPrepare::execute()
{
	if (m_paramBind)
	{
		if (mysql_stmt_bind_param(m_stmt, m_paramBind))
		{
			return -1;
		}
	}

	sql_stmt_lock.lock();
	if (mysql_stmt_execute(m_stmt))
	{
		sql_stmt_lock.unlock();
		return -1;
	}

	if (mysql_stmt_store_result(m_stmt))
	{
		sql_stmt_lock.unlock();
		return -1;
	}
	sql_stmt_lock.unlock();

	return static_cast<int>(mysql_stmt_affected_rows(m_stmt));
}

bool SqlPrepare::fetch()
{
	switch (mysql_stmt_fetch(m_stmt))
	{
	case 0:
	case MYSQL_DATA_TRUNCATED:
		m_ridx = 0;
		return true;
	case MYSQL_NO_DATA:
		return false;

	case 1: // the error
	default:
		ERROR_LOG(mysql_stmt_error(m_stmt));
	}

	return false;
}