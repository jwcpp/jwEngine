/************************************************************************
* @file      BasePacket.h
* @brief     Network packet base class
* @author    jiangwang
* @data      2019-1-15
* @version   0.1
************************************************************************/

#pragma once

#include "ByteBuffer.h"
#include "PoolObject.h"

class BasePacket : public ByteBuffer
{
public:
	BasePacket();
	~BasePacket();

	virtual void zero();
	virtual void release();
	virtual void moveData(BasePacket * packet);
	void shrink(int isize);

	virtual int32  getHeadSize();
	int32  getBodySize();
	const char * getBodyData();
	const char * readPointer();
	void setWriteSize(int size);
	int32 activeSize();
	// lua call blob
	std::string_view readData();
	void writeData(std::string_view sv);
	
	// read msg call
	virtual int32  getMarkLen();   // message head mark length
	virtual int getMsgType();
	virtual bool isHeadFull();

	// send msg call
	virtual int32  sendSize();
	virtual char * sendStream();

	int readPos();
	int writePos();
public:

	//------->get
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

	//------->push
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
protected:

	template<typename T>
	T popValue() {
		T t;
		*this >> t;
		return t;
	}

	template<typename T>
	T getValue(uint32 pos){
		T value;
		std::memcpy(&value, &(*_storage)[pos], sizeof(T));
		EndianConvert(value);

		return value;
	}

	template<typename T>
	void setValue(uint32 pos, T t){
		put(pos, t);
	}

	INCLUDE_POOL_OBJECT
};

