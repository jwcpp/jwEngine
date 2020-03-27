#ifndef _OBJECT_POOL_H
#define _OBJECT_POOL_H

#include <functional>
#include <list>
#include "XAssert.h"

template< typename T>
class ObjectPool
{
	typedef std::list<T*> OBJECTS;

public:
	ObjectPool(int32 maxcount = 0) :
		max_count(maxcount), total_allocs_(0), obj_count_(0)
	{
		
	}

	~ObjectPool(){
		destroy();
	}

	void init(int32 count)
	{
		max_count = count;
	}

	void destroy()
	{
		typename OBJECTS::iterator iter = objects_.begin();
		for (; iter != objects_.end(); ++iter)
		{
			(*iter)->setEnabledPoolObject(false);
			delete (*iter);
		}

		objects_.clear();
		obj_count_ = 0;
		total_allocs_ = 0;
	}

	T* createObject()
	{
		if (obj_count_ > 0)
		{
			T* t = *objects_.begin();
			objects_.pop_front();
			--obj_count_;
			t->setEnabledPoolObject(true);
			return t;
		}
		
		T* t = newObj();
		t->setEnabledPoolObject(true);
		++total_allocs_;
		return t;
	}

	virtual T * newObj()
	{
		return new T;
	}

	void reclaimObject(T* obj)
	{
		if (obj == NULL)
		{
			return;
		}
		if (!obj->isEnabledPoolObject())
		{
			XAssert(0, "The object is already in the pool");
			return;
		}

		obj->setEnabledPoolObject(false);
		if (obj_count_ >= max_count)
		{
			delete obj;
			--total_allocs_;
		}
		else
		{
			objects_.push_back(obj);
			++obj_count_;
		}
	}

	int32 getTotalAllocs(){ return total_allocs_; }
	int32 getObjCount(){ return obj_count_; }

private:
	int32 max_count;
	int32 total_allocs_;
	int32 obj_count_;
	OBJECTS objects_;
};


#endif