#include "WebSocketServer.h"
#include "WebSocketEvent.h"


WebSocketServer::WebSocketServer(EventLoop * loop, WebSocketEvent * wevent):
	TcpServer(loop),
	__m_event(wevent)
{
}


WebSocketServer::~WebSocketServer()
{
}


TcpSocket * WebSocketServer::createConnect()
{
	return (TcpSocket *)__m_event->createConnect();
}

void WebSocketServer::onConnect(TcpSocket * connect)
{
	__m_event->onConnect((WebSocketConnect *)connect);
}