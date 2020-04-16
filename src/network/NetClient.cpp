#include "NetClient.h"
#include "NetConnect.h"
#include "NetEvent.h"


NetClient::NetClient(EventLoop * loop, NetEvent * nevent) :
	TcpClient(loop),
	_netevent(nevent)
{

}

NetClient::~NetClient(){

}


TcpSocket * NetClient::createSocket()
{
	NetConnect * conn = _netevent->createConnect();
	return (TcpSocket *)conn;
}

void NetClient::onSocket(TcpSocket * conn, int argv)
{
	_netevent->onConnect((NetConnect *)conn, argv);
}