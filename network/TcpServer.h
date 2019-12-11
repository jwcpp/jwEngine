#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "TcpSocket.h"

#define DEFAULT_BACKLOG 128

class EventLoop;
class TcpServer
{
public:
	TcpServer(EventLoop * loop, uint32 buffersize = MESSAGE_BUFFER_SIZE);

	int listen(const char * ip, int port, bool ipv6 = false);

protected:
	
	virtual TcpSocket * createConnect();
	virtual void onConnect(TcpSocket * connect){};

private:
	static void on_new_connection(uv_stream_t *server, int status);

protected:
	uv_loop_t * mLoop;
	uv_tcp_t    m_uv_tcp;
	uint32      m_buffersize;
};

#endif