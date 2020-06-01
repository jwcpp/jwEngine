#include "XLog.h"
#include "TcpServer.h"
#include "EventLoop.h"

TcpServer::TcpServer(EventLoop * loop) :
	mLoop(loop->getLoop())
{
	m_uv_tcp.data = this;
	uv_tcp_init(mLoop, &m_uv_tcp);
}

TcpServer::~TcpServer()
{

}

int TcpServer::listen(const char * ip, int port, bool ipv6)
{

	/*
	#ifdef USE_IPV6_ADDR
	struct sockaddr_in6 mAddr;
	uv_ip6_addr(ip, port, &mAddr);
	#else
	struct sockaddr_in mAddr;
	uv_ip4_addr(ip, port, &mAddr);
	#endif
	*/
	sockaddr_storage iaddr;
	if (ipv6) {
		uv_ip6_addr(ip, port, (sockaddr_in6*)&iaddr);
	}
	else {
		uv_ip4_addr(ip, port, (sockaddr_in*)&iaddr);
	}

	uv_tcp_bind(&m_uv_tcp, (const struct sockaddr*)&iaddr, 0);
	return uv_listen((uv_stream_t*)&m_uv_tcp, DEFAULT_BACKLOG, on_new_connection);
}

void TcpServer::shutdown()
{
	uv_close((uv_handle_s *)&m_uv_tcp, [](uv_handle_t* handle){

	});
}

void TcpServer::on_new_connection(uv_stream_t *server, int status)
{
	if (status < 0) {
		ERROR_LOG("New connection error %s\n", uv_strerror(status));
		return;
	}

	TcpServer * self = (TcpServer *)server->data;
	// new connect
	TcpSocket * connect = self->createSocket();
	connect->clear();
	connect->init_uv_tcp(self->mLoop);
	if (connect->accept(server) == 0) {

		connect->on_read_start();
		self->onSocket(connect);
	}
	else {
		uv_close((uv_handle_t*)connect->getUvTcp(), NULL);
		ERROR_LOG("New connection close %s\n", uv_strerror(status));
		delete connect;
	}
}