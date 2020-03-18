/************************************************************************
* @file      WebSocketServer.h
* @brief     websocket 服务器，监听连接
* @author    jiangwang
* @data      2019-12-23
* @version   0.1
************************************************************************/

#pragma once

#include "TcpServer.h"

class WebSocketEvent;
class WebSocketServer : public TcpServer
{
public:
	WebSocketServer(EventLoop * loop, WebSocketEvent * wevent);
	~WebSocketServer();

protected:
	virtual TcpSocket * createSocket();
	virtual void onSocket(TcpSocket * connect);

private:
	WebSocketEvent * __m_event;
};

