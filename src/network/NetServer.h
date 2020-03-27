#ifndef NET_SERVER_H
#define NET_SERVER_H

#include "TcpServer.h"

class NetConnect;
class NetEvent;
class NetServer : public TcpServer
{
public:
	NetServer(EventLoop * loop, NetEvent * nevent);

protected:

	virtual TcpSocket * createSocket();
	virtual void onSocket(TcpSocket * connect);

private:
	NetEvent * _netevent;
};

#endif