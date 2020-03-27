/************************************************************************
* @file      UdpPacket.h
* @brief     udp message packet
* @author    jiangwang
* @data      2019-12-12
* @version   0.1
************************************************************************/

#pragma once

#include "BasePacket.h"
#include "PoolObject.h"

#define UDP_PACK_HEAD_SIZE sizeof(uint32)

class UdpPacket : public BasePacket
{
public:
	UdpPacket();
	~UdpPacket();

	void initSize(int size);
	uint32 getMsgType();

	virtual int32  getBodySize();
	virtual char * getBodyData();

	// read msg call
	virtual int32  getHeadSize();
	virtual int32  getMarkLen();   // message head mark length

	// send msg call
	virtual int32  sendSize();
	virtual char * sendStream();

	void readHead();
	void writeHead(int msgtype);
	void writeComplete(uint32 size);
	char * getData(){ return (char *)contents(); }

protected:
	void _fillHead();

	INCLUDE_POOL_OBJECT
};

