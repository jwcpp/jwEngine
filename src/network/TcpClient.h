#pragma once

#include "TcpSocket.h"

class EventLoop;
class TcpClient
{
public:
	TcpClient(EventLoop * loop);
	virtual ~TcpClient();

	virtual int connect(const char * ip, int port, bool ipv6 = false, int argv = 0);

protected:
	virtual TcpSocket * createSocket() = 0;
	virtual void onSocket(TcpSocket * conn, int argv) = 0;
protected:
	uv_loop_t * mLoop;
};

