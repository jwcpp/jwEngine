#include "RedisResult.h"
#include "hiredis.h"
#include "BasePacket.h"
#include <sstream>

RedisResult::RedisResult()
{
	m_reply = NULL;
	pos = 0;
}

RedisResult::~RedisResult()
{
	if(m_reply)
		freeReplyObject(m_reply);
}

void RedisResult::setResult(redisReply * result)
{
	m_reply = result;
}

bool RedisResult::isEmpty()
{
	return !(m_reply && (m_reply->str || m_reply->elements > 0));
}
bool RedisResult::fetch()
{
	if (pos >= getRowCount())
		return false;
	return true;
}

uint32 RedisResult::getRowCount()
{
	return m_reply->str ? 1 : m_reply->elements;
}

uint32 RedisResult::getFieldsCount()
{
	return getRowCount();
}

const char * RedisResult::getData(int & len)
{
	const char * str;
	// key or list
	if (m_reply->str)
	{
		len = m_reply->len;
		str = m_reply->str;
	}
	else
	{
		len = m_reply->element[pos]->len;
		str = m_reply->element[pos]->str;
	}

	pos++;
	return str;
}
const char * RedisResult::getData()
{
	const char * str;
	if (m_reply->str)
	{
		str = m_reply->str;
	}
	else
	{
		str = m_reply->element[pos]->str;
	}
	pos++;
	return str;
}

RedisResult &RedisResult::operator>>(std::string& value)
{
	if (pos >= getFieldsCount())
	{
		ERROR_LOG("redis row count upper limit");
		return *this;
	}

	// string
	std::istringstream ins(m_reply->str ? m_reply->str : m_reply->element[pos]->str);
	ins >> value;

	pos++;
	return *this;
}

int RedisResult::readBlob(BasePacket * packet)
{
	int len;
	const char * pdata = getData(len);
	if (len > 0)
	{
		packet->append((const uint8 *)pdata, len);
	}
	return len;
}

std::string_view RedisResult::getStream()
{
	int len;
	const char * p = getData(len);
	return std::string_view(p, len);
}