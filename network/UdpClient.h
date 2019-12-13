/************************************************************************
* @file      UdpClient.h
* @brief     udp client
* @author    jiangwang
* @data      2019-12-12
* @version   0.1
************************************************************************/

#pragma once
#include "libuv/include/uv.h"

class EventLoop;
class UdpClient
{
public:
	UdpClient(EventLoop * loop);
	virtual ~UdpClient();

	//int connect(const char * server_ip, int server_port, int port, bool ipv6 = false);
	int connect(const char * server_ip, int server_port, bool ipv6 = false);
	int writeToServer(char * buff, int len);

protected:
	virtual void onMessage(const sockaddr * addr, const char * buff, ssize_t len) = 0;

private:
	static void alloc_buffer(uv_handle_t * handle, size_t suggested_size, uv_buf_t * buf);
	static void on_read(uv_udp_t * handle, ssize_t nread, const uv_buf_t * rcvbuf, const sockaddr * addr, unsigned flags);

protected:
	uv_udp_t _udp;
	sockaddr_storage _server_addr;
};

