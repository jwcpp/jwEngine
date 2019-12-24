#include "WebSocketEvent.h"
#include "WebSocketConnect.h"


WebSocketEvent::WebSocketEvent()
{
}


WebSocketEvent::~WebSocketEvent()
{
}


WebSocketConnect * WebSocketEvent::createConnect()
{
	return new WebSocketConnect(this);
}

void WebSocketEvent::destroyConnect(WebSocketConnect * conn)
{
	if (conn)
		delete conn;
}