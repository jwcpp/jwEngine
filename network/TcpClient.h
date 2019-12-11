#pragma once

#include "TcpSocket.h"

class EventLoop;
class TcpClient
{
public:
	TcpClient(EventLoop * loop, uint32 buffersize = MESSAGE_BUFFER_SIZE);
	~TcpClient();

	virtual int connect(const char * ip, int port, bool ipv6 = false);

protected:
	virtual TcpSocket * createConnect();
	virtual void onConnect(TcpSocket * conn){};
protected:
	uv_loop_t * mLoop;
	int m_buffersize;
};

