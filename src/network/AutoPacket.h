#pragma once

#include "CommonPool.h"

template<typename T, typename MSG>
void readProto(T* buff, MSG & msg)
{
	const char * data = buff->getBodyData();
	msg.ParseFromArray((const void*)data, buff->getBodySize());
}

template<typename T, typename MSG>
void writeProto(T* buff, MSG & msg)
{
	int size = msg.ByteSizeLong();
	buff->setWriteSize(size);
	const char* data = buff->getBodyData();
	msg.SerializeToArray((void *)data, size);
}

template<typename T>
class AutoPacket
{
public:
	AutoPacket() {
		obj = CommPool::create<T>();
	}

	~AutoPacket()
	{
		CommPool::reclaim(obj);
	}

	T* operator *()
	{
		return obj;
	}

	template<typename MSG>

	T* writeProto(MSG& msg)
	{
		::writeProto<T, MSG>(obj, msg);
		return obj;
	}

private:

	T* obj;
};