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

private:

	T* obj;
};