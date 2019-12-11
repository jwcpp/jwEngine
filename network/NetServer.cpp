#include "NetServer.h"
#include "NetConnect.h"
#include "NetEvent.h"

NetServer::NetServer(EventLoop * loop, NetEvent * nevent, uint32 buffersize) :
	TcpServer(loop, buffersize),
	_netevent(nevent)
{
}

TcpSocket * NetServer::createConnect()
{
	return new NetConnect(_netevent, m_buffersize);
}

void NetServer::onConnect(TcpSocket * connect)
{
	_netevent->onAccept((NetConnect *)connect);
}