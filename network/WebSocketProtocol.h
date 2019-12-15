/************************************************************************
* @file      WebSocketProtocol.h
* @brief     websocket协议解析
* @author    jiangwang
* @data      2019-12-14
* @version   0.1
************************************************************************/

#pragma once

enum WSFrameType 
{
	ERROR_FRAME = 0xFF00,
	INCOMPLETE_FRAME = 0xFE00,

	OPENING_FRAME = 0x3300,
	CLOSING_FRAME = 0x3400,

	INCOMPLETE_TEXT_FRAME = 0x01,
	INCOMPLETE_BINARY_FRAME = 0x02,

	TEXT_FRAME = 0x81,
	BINARY_FRAME = 0x82,

	PING_FRAME = 0x19,
	PONG_FRAME = 0x1A
};

#include <string>

class WebSocketProtocol
{
public:
	WebSocketProtocol();
	~WebSocketProtocol();

	// 解析 WebSocket 的握手数据
	bool parseHandshake(const char * pData, int len);

	// 回客户端的握手协议
	std::string respondHandshake();

private:
	std::string __m_strKey;
	std::string __m_strHost;
};

