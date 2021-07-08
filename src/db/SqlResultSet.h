#pragma once

#include <stdio.h>
#include <string>

class SqlField
{
public:
	SqlField()
	{
		m_buff = NULL;
		m_len = 0;
	}

	~SqlField()
	{
		if (m_buff)
		{
			delete[] m_buff;
		}
	}

	void make(const char* pdata, int len)
	{
		m_len = len;
		m_buff = new char[len];
		memcpy(m_buff, pdata, len);
	}

	bool isEmpty()
	{
		return !m_buff;
	}

	const char* data() { return m_buff; }
	int length() { return m_len; }

	template<typename T>
	T getValue()
	{
		T val;
		memcpy(&val, m_buff, sizeof(T));
		return val;
	}

private:
	char* m_buff;
	int m_len;
};

#include <vector>
#include "BaseType.h"
#include "DBResult.h"

class BasePacket;
class SqlResultSet : public DBResult
{
public:
	SqlResultSet();
	~SqlResultSet();
	
	void setResult(MYSQL_STMT* stmt, MYSQL_RES* pResult, uint64 pRowCount, uint32 pFieldCount);
	virtual bool fetch();
	virtual bool isEmpty();
	virtual uint32 getRowCount();
	virtual uint32 getFieldsCount();
	virtual const char* getData(int& len);
	virtual const char* getData();

	SqlField* getField(int idx);
	bool emptyField(int idx);
	
	int8 getInt8();
	uint8 getUint8();
	int16 getInt16();
	uint16 getUint16();
	int32 getInt32();
	uint32 getUint32();
	int64 getInt64();
	uint64 getUint64();
	float getFloat();
	double getDouble();
	std::string getString();
	int readBlob(BasePacket* packet);
	std::string_view getStrview(); // lua call

protected:
	bool _fetch(MYSQL_STMT * stmt);
private:
	std::vector<SqlField> m_fields;
	int64 m_currRowIdx;
	int32 m_currColIdx;
	int64 m_rowCount;
	int32 m_fieldCount;
};

