#ifndef NET_SERVER_H
#define NET_SERVER_H

#include "TcpServer.h"

class NetConnect;
class NetEvent;
class NetServer : public TcpServer
{
public:
	NetServer(EventLoop * loop, NetEvent * nevent, uint32 buffersize = MESSAGE_BUFFER_SIZE);

protected:

	virtual TcpSocket * createConnect();
	virtual void onConnect(TcpSocket * connect);

private:
	NetEvent * _netevent;
};

#endif