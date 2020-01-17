/************************************************************************
* @file      BasePacket.h
* @brief     Network packet base class
* @author    jiangwang
* @data      2019-1-15
* @version   0.1
************************************************************************/

#pragma once

#include "ByteBuffer.h"

class BasePacket : public ByteBuffer
{
public:
	BasePacket();
	~BasePacket();

	virtual void zero();
	virtual void moveData(BasePacket * packet);

	virtual int32  getBodySize();
	virtual char * getBodyData();
	
	// read msg call
	virtual int32  getHeadSize();
	virtual int32  getMarkLen();   // message head mark length

	// send msg call
	virtual int32  sendSize();
	virtual char * sendStream();

protected:
	virtual void _fillHead();

	template<typename T>
	T getValue(uint32 pos){
		T value;
		std::memcpy(&value, &_storage[pos], sizeof(T));
		EndianConvert(value);

		return value;
	}

	template<typename T>
	void setValue(uint32 pos, T t){
		put(pos, t);
	}
};

