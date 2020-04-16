#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H


#include "TcpClient.h"

class NetEvent;
class NetClient : public TcpClient
{

public:
	NetClient(EventLoop * loop, NetEvent * nevent);

	~NetClient();

protected:
	virtual TcpSocket * createSocket();
	virtual void onSocket(TcpSocket * conn, int argv);

private:
	NetEvent * _netevent;
};

#endif