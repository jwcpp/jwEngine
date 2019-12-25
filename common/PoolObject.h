#pragma once

//include body
#define INCLUDE_POOL_OBJECT\
	public:\
		bool isEnabledPoolObject() const{ return isEnabledPoolObject_; }\
		void setEnabledPoolObject(bool v){ isEnabledPoolObject_ = v; }\
	private:\
		bool isEnabledPoolObject_ = true;

class PoolObject
{
	INCLUDE_POOL_OBJECT
};