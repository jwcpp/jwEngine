#include "UdpServer.h"
#include "EventLoop.h"
#include "XLog.h"

UdpServer::UdpServer(EventLoop * loop)
{
	_m_udp.data = this;
	uv_udp_init(loop->getLoop(), &_m_udp);
}


UdpServer::~UdpServer()
{
}

int UdpServer::start(const char * ip, int port, bool ipv6)
{

	sockaddr_storage iaddr;
	if (ipv6) {
		uv_ip6_addr(ip, port, (sockaddr_in6*)&iaddr);
	}
	else {
		uv_ip4_addr(ip, port, (sockaddr_in*)&iaddr);
	}

	int errcode = uv_udp_bind(&_m_udp, (const struct sockaddr *)&iaddr, UV_UDP_REUSEADDR);
	if (errcode)
	{
		ERROR_LOG("uv_udp_bind error %s\n", uv_strerror(errcode));
		return -1;
	}

	errcode = uv_udp_recv_start(&_m_udp, alloc_buffer, on_read);
	if (errcode)
	{
		ERROR_LOG("uv_udp_recv_start error %s\n", uv_strerror(errcode));
		return -1;
	}

	return 0;
}

void UdpServer::alloc_buffer(uv_handle_t * handle, size_t suggested_size, uv_buf_t * buf)
{
	static const int bufflen = 65536;
	static char buff[bufflen];

	buf->base = buff;
	buf->len = bufflen;

}

void UdpServer::on_read(uv_udp_t * handle, ssize_t nread, const uv_buf_t * rcvbuf, const sockaddr * addr, unsigned flags)
{
	UdpServer * server = (UdpServer *)handle->data;
	server->onMessage(addr, rcvbuf->base, nread);
}