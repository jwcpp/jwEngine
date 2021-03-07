#pragma once
/************************************************************************
* @file      RedisResult.h
* @brief     redis result
* @author    jiangwang
* @data      2020-3-31
* @version   0.1
************************************************************************/

#include "XLog.h"
#include "BaseType.h"
#include "DBResult.h"
#include <cstring>

class BasePacket;
struct redisReply;
class RedisResult : public DBResult
{
public:
	RedisResult();
	~RedisResult();
	void setResult(redisReply * result);

	virtual bool isEmpty();
	virtual bool fetch();
	virtual uint32 getRowCount();
	virtual uint32 getFieldsCount();

	virtual const char * getData(int & len);
	virtual const char * getData();

	// base type
	template<typename T>
	RedisResult & operator >> (T & t)
	{
		if (pos >= getFieldsCount())
		{
			ERROR_LOG("redis row count upper limit");
			return *this;
		}

		// blob
		const char * p = m_reply->str ? m_reply->str : m_reply->element[pos]->str;
		std::memcpy(&t, p, sizeof(t));

		pos++;
		return *this;
	}

	// string type
	RedisResult &operator>>(std::string& value);
	int readBlob(BasePacket * packet);
	std::string_view getStream();
private:
	redisReply * m_reply;
	uint32 pos;
};

