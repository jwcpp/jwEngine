#include "BaseType.h"
#include <string>
#include "mysql.h"

#include "SqlPrepare.h"
#include "Tools.h"
#include "XLog.h"
#include "BasePacket.h"
#include <assert.h>
#include "SqlResultSet.h"

SqlPrepare::SqlPrepare(const char * sqlstr)
{
	m_stmt = NULL;
	m_paramBind = NULL;
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
	if (m_stmt)
	{
		if (mysql_stmt_close(m_stmt))
		{
			ERROR_LOG(mysql_stmt_error(m_stmt));
		}
		m_stmt = NULL;
	}

	if (m_paramBind != NULL)
	{
		for (int i = 0; i < m_count; ++i)
		{
			MYSQL_BIND* pBind = &m_paramBind[i];
			if(pBind->buffer) delete [] pBind->buffer;
		}

		delete[]m_paramBind;
		delete[]m_paramLengths;
		m_paramBind = NULL;
		m_paramLengths = NULL;
	}
}

void SqlPrepare::clear()
{
	m_widx = 0;
}

static void SetParameterValue(MYSQL_BIND* param, enum_field_types type, const void* value, uint32 len, bool isUnsigned)
{
	if (param->buffer) delete[] param->buffer;

	param->buffer_type = type;
	param->buffer = new char[len];
	param->buffer_length = *param->length = len;
	param->is_null_value = 0;
	param->is_unsigned = isUnsigned;

	memcpy(param->buffer, value, len);
}
void SqlPrepare::pushInt8(int8 value)
{
	MYSQL_BIND* pBind = &m_paramBind[m_widx++];
	SetParameterValue(pBind, MYSQL_TYPE_TINY, &value, sizeof(value), false);
}

void SqlPrepare::pushUint8(uint8 value)
{
	MYSQL_BIND* pBind = &m_paramBind[m_widx++];
	SetParameterValue(pBind, MYSQL_TYPE_TINY, &value, sizeof(value), true);
}
void SqlPrepare::pushInt16(int16 value)
{
	MYSQL_BIND* pBind = &m_paramBind[m_widx++];
	SetParameterValue(pBind, MYSQL_TYPE_SHORT, &value, sizeof(value), false);
}
void SqlPrepare::pushUint16(uint16 value)
{
	MYSQL_BIND* pBind = &m_paramBind[m_widx++];
	SetParameterValue(pBind, MYSQL_TYPE_SHORT, &value, sizeof(value), true);
}
void SqlPrepare::pushInt32(int32 value)
{
	MYSQL_BIND* pBind = &m_paramBind[m_widx++];
	SetParameterValue(pBind, MYSQL_TYPE_LONG, &value, sizeof(value), false);
}
void SqlPrepare::pushUint32(uint32 value)
{
	MYSQL_BIND* pBind = &m_paramBind[m_widx++];
	SetParameterValue(pBind, MYSQL_TYPE_LONG, &value, sizeof(value), true);
}
void SqlPrepare::pushInt64(int64 value)
{
	MYSQL_BIND* pBind = &m_paramBind[m_widx++];
	SetParameterValue(pBind, MYSQL_TYPE_LONGLONG, &value, sizeof(value), false);
}
void SqlPrepare::pushUint64(uint64 value)
{
	MYSQL_BIND* pBind = &m_paramBind[m_widx++];
	SetParameterValue(pBind, MYSQL_TYPE_LONGLONG, &value, sizeof(value), true);
}
void SqlPrepare::pushFloat(float value)
{
	MYSQL_BIND* pBind = &m_paramBind[m_widx++];
	SetParameterValue(pBind, MYSQL_TYPE_FLOAT, &value, sizeof(value), (value > 0.0f));
}
void SqlPrepare::pushDouble(double value)
{
	MYSQL_BIND* pBind = &m_paramBind[m_widx++];
	SetParameterValue(pBind, MYSQL_TYPE_DOUBLE, &value, sizeof(value), (value > 0.0f));
}
void SqlPrepare::pushString(std::string value)
{
	MYSQL_BIND* pBind = &m_paramBind[m_widx++];

	int len = value.length();

	pBind->buffer_length = *pBind->length = len;
	pBind->buffer_type = MYSQL_TYPE_STRING;
	pBind->is_unsigned = 0;

	if (pBind->buffer == NULL)
		pBind->buffer = new char[len];
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
		pBind->buffer = new char[len];
	memcpy(pBind->buffer, pData, len);
}

int SqlPrepare::prepare(MYSQL * mysql)
{
	m_stmt = mysql_stmt_init(mysql);
	if (!m_stmt)
	{
		return -1;
	}

	if (mysql_stmt_prepare(m_stmt, m_sql.data(), static_cast<unsigned long>(m_sql.length())))
	{
		return -1;
	}

	if (mysql_stmt_param_count(m_stmt) != m_count)
	{
		ERROR_LOG("mysql_stmt_param_count(m_stmt) != m_count");
		assert(0);
	}

	my_bool bool_tmp = my_bool(1);
	mysql_stmt_attr_set(m_stmt, STMT_ATTR_UPDATE_MAX_LENGTH, &bool_tmp);

	return 0;
}
int SqlPrepare::execute(DBResult* resultSet)
{
	MYSQL_RES* result = NULL;
	if(!_query(&result)) return -1;

	if (result)
	{
		std::shared_ptr<void> free_res(nullptr, [result](void*) { mysql_free_result(result); });

		if (resultSet)
		{
			if (mysql_stmt_store_result(m_stmt)) return -1;

			static_cast<SqlResultSet*>(resultSet)->setResult(m_stmt, result, mysql_stmt_num_rows(m_stmt), mysql_stmt_field_count(m_stmt));
		}
	}

	return static_cast<int>(mysql_stmt_affected_rows(m_stmt));
}

bool SqlPrepare::_query(MYSQL_RES** pRes)
{
	if (m_paramBind)
	{
		if (mysql_stmt_bind_param(m_stmt, m_paramBind))
		{
			return false;
		}
	}

	if (mysql_stmt_execute(m_stmt))
	{
		return false;
	}

	*pRes = mysql_stmt_result_metadata(m_stmt);
	return true;
}