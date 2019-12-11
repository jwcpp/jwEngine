#include "NetClient.h"
#include "NetConnect.h"
#include "NetEvent.h"


NetClient::NetClient(EventLoop * loop, NetEvent * nevent, uint32 buffersize) :
TcpClient(loop, buffersize),
_netevent(nevent)
{

}

NetClient::~NetClient(){

}


TcpSocket * NetClient::createConnect()
{
	return new NetConnect(_netevent, m_buffersize);
}

void NetClient::onConnect(TcpSocket * conn)
{
	_netevent->onConnect((NetConnect *)conn);
}