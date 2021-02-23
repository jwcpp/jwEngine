/************************************************************************
* @file      UdpPacket.h
* @brief     udp message packet
* @author    jiangwang
* @data      2019-12-12
* @version   0.1
************************************************************************/

#pragma once

#include "BasePacket.h"

#define UDP_PACK_HEAD_SIZE sizeof(uint32)

class UdpPacket : public BasePacket
{
public:
	UdpPacket();
	~UdpPacket();

	virtual void zero();
	void initSize(int size);

	virtual int32  getBodySize();
	virtual char * getBodyData();
	virtual void setWriteSize(int size);

	// read msg call
	virtual int32  getHeadSize();
	virtual int32  getMarkLen();   // message head mark length
	virtual int getMsgType();

	// send msg call
	virtual int32  sendSize();
	virtual char * sendStream();

	void writeHead(int msgtype);
	void writeComplete(uint32 size);
};

