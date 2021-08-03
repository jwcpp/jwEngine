#pragma once
/************************************************************************
* @file      SqlPrepare.h
* @brief     sql prepare
* @author    jiangwang
* @data      2020-3-23
* @version   0.1
************************************************************************/

class BasePacket;
class DBResult;
class SqlPrepare
{
public:
	SqlPrepare(const char * sqlstr);
	~SqlPrepare();

	void clear();

	// push
	void pushInt8(int8 value);
	void pushUint8(uint8 value);
	void pushInt16(int16 value);
	void pushUint16(uint16 value);
	void pushInt32(int32 value);
	void pushUint32(uint32 value);
	void pushInt64(int64 value);
	void pushUint64(uint64 value);
	void pushFloat(float value);
	void pushDouble(double value);
	void pushString(std::string value);
	void pushBlob(BasePacket * packet);
	void pushData(std::string_view sv); // lua call
	
	void write(void* pData, int len);

	int prepare(MYSQL * mysql);
	int execute(DBResult* result = NULL);
protected:
	bool _query(MYSQL_RES** pRes);
private:
	int m_count;
	std::string m_sql;
	MYSQL_STMT * m_stmt;
	MYSQL_BIND * m_paramBind;
	unsigned long * m_paramLengths;

private:
	int m_widx;  // write
};