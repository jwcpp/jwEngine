#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H


#include "TcpClient.h"

class NetEvent;
class NetClient : public TcpClient
{

public:
	NetClient(EventLoop * loop, NetEvent * nevent, uint32 buffersize = MESSAGE_BUFFER_SIZE);

	~NetClient();

protected:
	virtual TcpSocket * createConnect();
	virtual void onConnect(TcpSocket * conn);

private:
	NetEvent * _netevent;
};

#endif