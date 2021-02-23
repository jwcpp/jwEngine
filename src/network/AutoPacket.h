#pragma once

#include "CommonPool.h"

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
	T* writeProto(MSG & msg)
	{
		msg.SerializeToArray(pack.GetMsgBody(), msg.ByteSize());
		return obj;
	}

private:

	T* obj;
};