/************************************************************************
* @file      WebSocketPacket.h
* @brief     websocket协议包
* @author    jiangwang
* @data      2019-12-16
* @version   0.1
************************************************************************/

#pragma once
#include "ByteBuffer.h"
#include "PoolObject.h"

#define WS_HEAD_SIZE 2
#define WS_RESERVER_SIZE (WS_HEAD_SIZE + 8)

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

class WebSocketPacket : public ByteBuffer
{
public:
	WebSocketPacket();
	~WebSocketPacket();

	void zero();
	uint8 getFin(){ return (__m_head[0] >> 7) & 0x01; }
	uint8 getOpcode(){ return __m_head[0] & 0x0F; }
	uint8 getMask(){ return (__m_head[1] >> 7) & 0x01; }
	uint8 getPayloadLen(){ return __m_head[1] & 0x7f; }

	uint32 getHeadSize();
	bool isHeadFull();
	void writeFrameHead(WSFrameType ftype = BINARY_FRAME);
	uint32 readFrameHead(const uint8 * p, uint32 size);
	uint32 getLength();
	char * sendStream(){ return (char *)contents() + __m_sendpos; }
	uint32 sendLen(){ return _wpos - __m_sendpos; }
	uint32 getMaskKey();
	const char * getBodyData();
private:

	void fillPacketHead();

	template<typename T>
	T getValue(uint32 pos){
		T value;
		std::memcpy(&value, &_storage[pos], sizeof(T));
		EndianConvert(value);

		return value;
	}

private:
	uint8 __m_head[WS_HEAD_SIZE];
	uint8 __m_sendpos;
	INCLUDE_POOL_OBJECT
};

