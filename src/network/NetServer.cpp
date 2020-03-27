#include "NetServer.h"
#include "NetConnect.h"
#include "NetEvent.h"

NetServer::NetServer(EventLoop * loop, NetEvent * nevent) :
	TcpServer(loop),
	_netevent(nevent)
{
}

TcpSocket * NetServer::createSocket()
{
	NetConnect * conn = _netevent->createConnect();
	return (TcpSocket *)conn;
}

void NetServer::onSocket(TcpSocket * connect)
{
	_netevent->onAccept((NetConnect *)connect);
}