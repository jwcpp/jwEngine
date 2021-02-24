/************************************************************************
* @file      WebSocketPacket.h
* @brief     websocket协议包
* @author    jiangwang
* @data      2019-12-16
* @version   0.1
************************************************************************/

#pragma once
#include "BasePacket.h"

#define WS_HEAD_SIZE 2
#define WS_MAX_HEAD_SIZE 14

/*

0                   1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-------+-+-------------+-------------------------------+
|F|R|R|R| opcode|M| Payload len |    Extended payload length    |
|I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
|N|V|V|V|       |S|             |   (if payload len==126/127)   |
| |1|2|3|       |K|             |                               |
+-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +

*/

enum WSFrameType
{
	ERROR_FRAME = 0xFF00,
	INCOMPLETE_FRAME = 0xFE00,

	OPENING_FRAME = 0x3300,
	CLOSING_FRAME = 0x3400,

	INCOMPLETE_TEXT_FRAME = 0x01,
	INCOMPLETE_BINARY_FRAME = 0x02,

	// 文本帧与二进制帧
	TEXT_FRAME = 0x81,
	BINARY_FRAME = 0x82,

	// ping pong
	PING_FRAME = 0x89,
	PONG_FRAME = 0x8A,

	// 关闭连接
	CLOSE_FRAME = 0x08
};

class WebSocketPacket : public BasePacket
{
public:
	WebSocketPacket();
	~WebSocketPacket();

	virtual void zero();
	virtual int32  getHeadSize();
	virtual void moveData(BasePacket* packet);

	uint8 getFin(){ return (__m_head[0] >> 7) & 0x01; }
	uint8 getOpcode(){ return __m_head[0] & 0x0F; }
	uint8 getMask(){ return (__m_head[1] >> 7) & 0x01; }
	uint8 getPayloadLen(){ return __m_head[1] & 0x7f; }

	void writeFrameHead(int32 bodylen, WSFrameType ftype = BINARY_FRAME);
	uint32 readFrameHead(const uint8 * p, uint32 size);
	uint32 getMaskKey();

	// read msg call
	virtual int32  getMarkLen();   // message head mark length
	virtual bool isHeadFull();

	// send msg call
	virtual int32  sendSize();
	virtual char * sendStream();

	void setPongPacket();
private:
	int32 calcHeadSize();
private:
	uint8 __m_head[WS_HEAD_SIZE];
	uint8 __m_headpos;
};

