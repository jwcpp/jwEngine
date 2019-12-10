#include "NetEvent.h"
#include "NetConnect.h"

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