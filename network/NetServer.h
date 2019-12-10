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

	virtual TcpSocket * createConnect();
	virtual void onConnect(TcpSocket * connect);

private:
	NetEvent * _netevent;
};

#endif