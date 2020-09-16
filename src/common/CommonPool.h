#pragma once
/************************************************************************
* @file      CommonPool.h
* @brief     数据包对象池
* @author    jiangwang
* @data      2020-3-17
* @version   0.1
************************************************************************/

#include "BaseType.h"
#include "Objectpool.h"
#include "Singleton.h"

template<typename T>
class CommonPool : public Singleton<CommonPool<T>>
{
public:
	ObjectPool<T> objpool;
};

namespace CommPool
{
	template<typename T>
	void init(int count)
	{
		CommonPool<T>::Instance()->objpool.init(count);
	}

	template<typename T>
	T * create()
	{
		T * obj = CommonPool<T>::Instance()->objpool.createObject();
		obj->zero();
		return obj;
	}

	template<typename T>
	void reclaim(T * obj)
	{
		obj->release();
		CommonPool<T>::Instance()->objpool.reclaimObject(obj);
	}

	template<typename T>
	void destroy()
	{
		CommonPool<T>::Instance()->objpool.destroy();
	}

	template<typename T>
	std::list<T*> & getobjs()
	{
		return CommonPool<T>::Instance()->objpool.getObjs();
	}
}