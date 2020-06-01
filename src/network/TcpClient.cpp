#include "TcpClient.h"
#include "XLog.h"
#include "EventLoop.h"

TcpClient::TcpClient(EventLoop * loop) :
	mLoop(loop->getLoop())
{
	
}


TcpClient::~TcpClient()
{
}

int TcpClient::connect(const char * ip, int port, bool ipv6, int argv)
{
	struct user_connect : public uv_connect_t
	{
		TcpSocket * conn;
		int argv;
	};

	user_connect * user_conn = new user_connect;
	user_conn->conn = createSocket();
	user_conn->data = this;
	user_conn->argv = argv;

	uv_tcp_init(mLoop, user_conn->conn->getUvTcp());

	sockaddr_storage iaddr;
	if (ipv6) {
		uv_ip6_addr(ip, port, (sockaddr_in6*)&iaddr);
	}
	else {
		uv_ip4_addr(ip, port, (sockaddr_in*)&iaddr);
	}

	return uv_tcp_connect(user_conn, user_conn->conn->getUvTcp(), (sockaddr*)&iaddr,
		[](uv_connect_t* req, int status) {
		user_connect *user_conn = (user_connect*)req;
		TcpClient * self = (TcpClient *)user_conn->data;

		if (status < 0) {
			uv_close((uv_handle_t*)(user_conn->conn->getUvTcp()), NULL);
			self->onSocket(NULL, user_conn->argv);
		}
		else {
			user_conn->conn->on_read_start();
			self->onSocket(user_conn->conn, user_conn->argv);
		}

		delete user_conn;
	});
}