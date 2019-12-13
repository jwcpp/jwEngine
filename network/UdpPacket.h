/************************************************************************
* @file      UdpPacket.h
* @brief     udpÊý¾Ý°ü
* @author    jiangwang
* @data      2019-12-12
* @version   0.1
************************************************************************/

#pragma once

#include "ByteBuffer.h"
#include "PoolObject.h"

#define UDP_PACK_HEAD_SIZE sizeof(uint32)

class UdpPacket : public ByteBuffer
{
public:
	UdpPacket();
	~UdpPacket();

	void zero();
	void initSize(int size);

	void setType(uint32 type){ __m_MsgType = type; }
	uint32 getType(){ return __m_MsgType; }

	void readHead();
	void writeHead();
	void writeComplete(uint32 size);
	char * getData(){ return (char *)contents(); }

private:
	template<typename T>
	T __getValue(uint32 pos){
		T value;
		std::memcpy(&value, &_storage[pos], sizeof(T));
		EndianConvert(value);

		return value;
	}

	void __fillPacketHead();

private:
	uint32 __m_MsgType;

	INCLUDE_POOL_OBJECT
};

