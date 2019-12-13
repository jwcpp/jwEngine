#include "NetEvent.h"
#include "NetConnect.h"

NetEvent::NetEvent()
{

}

NetEvent::~NetEvent()
{

}

NetConnect * NetEvent::createConnect()
{ 
	return new NetConnect(this, MESSAGE_BUFFER_SIZE);
}

void NetEvent::destroyConnect(NetConnect * conn)
{
	if (conn)
	{
		delete conn;
	}
}