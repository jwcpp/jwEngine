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


TcpSocket * NetClient::createConnect()
{
	NetConnect * conn = _netevent->createConnect();
	return (TcpSocket *)conn;
}

void NetClient::onConnect(TcpSocket * conn)
{
	_netevent->onConnect((NetConnect *)conn);
}