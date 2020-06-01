#include "UdpClient.h"
#include "EventLoop.h"
#include "XLog.h"

UdpClient::UdpClient(EventLoop * loop)
{
	_udp.data = this;
	uv_udp_init(loop->getLoop(), &_udp);
}


UdpClient::~UdpClient()
{
}

/*
int UdpClient::connect(const char * server_ip, int server_port, int port, bool ipv6)
{
	int err_code = 0;

	//sockaddr_storage _server_addr;
	if (ipv6)
	err_code = uv_ip6_addr(server_ip, server_port, (sockaddr_in6 *)&_server_addr);
	else
	err_code = uv_ip4_addr(server_ip, server_port, (sockaddr_in *)&_server_addr);

	if (err_code < 0)
	{
	ERROR_LOG("uv_ip4_addr error %s\n", uv_strerror(err_code));
	return -1;
	}

	sockaddr_storage _listen_addr;
	if (ipv6)
	err_code = uv_ip6_addr("0.0.0.0", port, (sockaddr_in6 *)&_listen_addr);
	else
	err_code = uv_ip4_addr("0.0.0.0", port, (sockaddr_in *)&_listen_addr);

	if (err_code < 0)
	{
	ERROR_LOG("uv_ip4_addr error %s\n", uv_strerror(err_code));
	return -2;
	}

	err_code = uv_udp_bind(&_udp, (sockaddr *)&_listen_addr, 0);
	if (err_code < 0)
	{
	ERROR_LOG("uv_udp_bind error %s\n", uv_strerror(err_code));
	return -3;
	}
	err_code = uv_udp_recv_start(&_udp, UdpClient::alloc_buffer, UdpClient::on_read);
	if (err_code < 0)
	{
	ERROR_LOG("uv_udp_recv_start error %s\n", uv_strerror(err_code));
	return -4;
	}

	return 0;
}
*/

int UdpClient::connect(const char * server_ip, int server_port, bool ipv6)
{
	int err_code = 0;

	//sockaddr_storage _server_addr;
	if (ipv6)
		err_code = uv_ip6_addr(server_ip, server_port, (sockaddr_in6 *)&_server_addr);
	else
		err_code = uv_ip4_addr(server_ip, server_port, (sockaddr_in *)&_server_addr);

	if (err_code < 0)
	{
		ERROR_LOG("uv_ip4_addr error %s\n", uv_strerror(err_code));
		return -1;
	}

	err_code = uv_udp_recv_start(&_udp, UdpClient::alloc_buffer, UdpClient::on_read);
	if (err_code < 0)
	{
		ERROR_LOG("uv_udp_recv_start error %s\n", uv_strerror(err_code));
		return -2;
	}

	return 0;
}

void UdpClient::alloc_buffer(uv_handle_t * handle, size_t suggested_size, uv_buf_t * buf)
{
	static const int bufflen = 65536;
	static char buff[bufflen];

	buf->base = buff;
	buf->len = bufflen;
}

void UdpClient::on_read(uv_udp_t * handle, ssize_t nread, const uv_buf_t * rcvbuf, const sockaddr * addr, unsigned flags)
{
	UdpClient * self = static_cast<UdpClient *>(handle->data);
	do
	{
		if (nread <= 0)
		{
			break;
		}

		self->onMessage(addr, rcvbuf->base, nread);

	} while (false);
}

int UdpClient::writeToServer(char * buff, int len)
{
	uv_buf_t msg = uv_buf_init(buff, len);
	int send_len = uv_udp_try_send(&_udp, &msg, 1, (const struct sockaddr*)&_server_addr);
	return send_len;
}