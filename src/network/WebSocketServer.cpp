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


TcpSocket * WebSocketServer::createSocket()
{
	return (TcpSocket *)__m_event->createConnect();
}

void WebSocketServer::onSocket(TcpSocket * connect)
{
	__m_event->onAccept((WebSocketConnect *)connect);
}