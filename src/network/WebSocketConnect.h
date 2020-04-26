/************************************************************************
* @file      WebSocketConnect.h
* @brief     websocket协议解析
* @author    jiangwang
* @data      2019-12-14
* @version   0.1
************************************************************************/

#pragma once

#include <string>
#include <queue>
#include "TcpServer.h"

class WebSocketEvent;
class WebSocketPacket;
class WebSocketConnect : public TcpSocket
{
public:
	WebSocketConnect(WebSocketEvent * wevent, uint32 buffersize = MESSAGE_BUFFER_SIZE);
	~WebSocketConnect();

	// 解析 WebSocket 的握手数据
	bool parseHandshake(const char * pData, int len);

	// 回客户端的握手协议
	void respondHandshake(std::string & msg);

	void sendMsg(WebSocketPacket * pack);
	void sendMsg(void * msg, uint32 len);
	void sendPacket(WebSocketPacket * pack) { sendMsg(pack); }
	void sendData(void * msg, uint32 len) { sendMsg(msg, len); }
protected:

	virtual void on_msgbuffer(MessageBuffer * buffer);
	virtual void on_clsesocket();
	virtual void on_writecomplete();

private:
	WebSocketPacket * createPacket();
	void recyclePacket(WebSocketPacket * pack);
	void send_top_msg();
	bool decodingDatas(WebSocketPacket* pPacket, uint32 msg_mask);
private:
	std::string __m_strKey;
	std::string __m_strHost;
	std::string __m_handshakeMsg;
	bool __m_isHandshake;
	WebSocketEvent * __m_webevent;
	WebSocketPacket * __m_readPacket;
	std::queue<WebSocketPacket *> __m_sendPackets;
};

