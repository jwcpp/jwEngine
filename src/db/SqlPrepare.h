#pragma once
/************************************************************************
* @file      SqlPrepare.h
* @brief     sql prepare
* @author    jiangwang
* @data      2020-3-23
* @version   0.1
************************************************************************/

class BasePacket;
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

	//get
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
	int readBlob(BasePacket * packet);
	std::string getData(); // lua call

	int fieldLen();
	int read(void * pData);

	int prepare(MYSQL * mysql);
	int execute();
	bool fetch();

private:
	void allocateParamBuffer(MYSQL_BIND* bind);
	void allocateResultBuffer(MYSQL_BIND* bind, MYSQL_FIELD *field);

	std::string m_sql;
	MYSQL_STMT * m_stmt;
	int m_count;
	int m_field;
	MYSQL_BIND * m_paramBind;
	MYSQL_BIND * m_resultBind;
	unsigned long * m_paramLengths;
	unsigned long * m_resultLengths;
	my_bool * m_isNulls;

private:
	int m_ridx;  // read
	int m_widx;  // write
};