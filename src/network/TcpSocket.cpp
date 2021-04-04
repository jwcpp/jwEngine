#include "XLog.h"
#include "TcpSocket.h"


TcpSocketBase::TcpSocketBase(uint32 buffersize)
{
	m_uv_tcp.data = this;
	m_write_t.data = this;
	mBuffer.Resize(buffersize);
}

TcpSocketBase::~TcpSocketBase()
{

}

void TcpSocketBase::clear()
{
	mBuffer.Reset();
}

void TcpSocketBase::close()
{
	uv_close((uv_handle_t *)getUvTcp(), on_uv_close);
}


//void TcpSocket::createConnect(uv_loop_t * loop, const char * ip, int port, std::function<void(std::shared_ptr<TcpSocket> &)> cb, bool ipv6)
//{
//	
//	struct user_connect : public uv_connect_t
//	{
//	std::shared_ptr<TcpSocket> connobj;
//	std::function<void(std::shared_ptr<TcpSocket>)> cb;
//	};
//
//	std::shared_ptr<TcpSocket> connobj(new TcpSocket());
//	uv_tcp_init(loop, connobj->getUvTcp());
//
//	sockaddr_storage iaddr;
//	if (ipv6) {
//	uv_ip6_addr(ip, port, (sockaddr_in6*)&iaddr);
//	}
//	else {
//	uv_ip4_addr(ip, port, (sockaddr_in*)&iaddr);
//	}
//
//	user_connect * uconn = new user_connect;
//	uconn->connobj = connobj;
//	uconn->cb = cb;
//	uv_tcp_connect(uconn, connobj->getUvTcp(), (sockaddr*)&iaddr,
//	[](uv_connect_t* req, int status) {
//	user_connect *uconn = (user_connect*)req;
//	std::shared_ptr<TcpSocket> connobj = uconn->connobj;
//	auto cb = uconn->cb;
//
//	delete uconn;
//
//	if (status < 0) {
//	uv_close((uv_handle_t*)(connobj->getUvTcp()), NULL);
//	cb(nullptr);
//	}
//	else {
//	cb(connobj);
//	}
//	});
//	
//}

void TcpSocketBase::on_read_start()
{
	uv_read_start((uv_stream_t *)getUvTcp(), alloc_buffer, echo_read);
}

std::string TcpSocketBase::localIP() const
{
	sockaddr_storage addr;
	int addr_len = sizeof(addr);
	uv_tcp_getsockname(&m_uv_tcp, (sockaddr*)&addr, &addr_len);

	char endpoint[64] = { 0 };
	if (addr.ss_family == AF_INET) {
		uv_ip4_name((const sockaddr_in*)&addr, endpoint, 64);
	}
	else if (addr.ss_family == AF_INET6) {
		uv_ip6_name((const sockaddr_in6*)&addr, endpoint, 64);
	}

	return endpoint;
}

int TcpSocketBase::localPort() const
{
	sockaddr_storage addr;
	int addr_len = sizeof(addr);
	uv_tcp_getsockname(&m_uv_tcp, (sockaddr*)&addr, &addr_len);

	char endpoint[64] = { 0 };
	if (addr.ss_family == AF_INET) {
		sockaddr_in *s = (sockaddr_in*)&addr;
		return ntohs(s->sin_port);
	}
	else if (addr.ss_family == AF_INET6) {
		sockaddr_in6 *s = (sockaddr_in6*)&addr;
		return ntohs(s->sin6_port);
	}

	return 0;
}

std::string TcpSocketBase::remoteIP() const
{
	sockaddr_storage addr;
	int addr_len = sizeof(addr);
	uv_tcp_getpeername(&m_uv_tcp, (sockaddr*)&addr, &addr_len);

	char endpoint[64] = { 0 };
	if (addr.ss_family == AF_INET) {
		uv_ip4_name((const sockaddr_in*)&addr, endpoint, 64);
	}
	else if (addr.ss_family == AF_INET6) {
		uv_ip6_name((const sockaddr_in6*)&addr, endpoint, 64);
	}

	return endpoint;
}

int TcpSocketBase::remotePort() const
{
	sockaddr_storage addr;
	int addr_len = sizeof(addr);
	uv_tcp_getpeername(&m_uv_tcp, (sockaddr*)&addr, &addr_len);

	char endpoint[64] = { 0 };
	if (addr.ss_family == AF_INET) {
		sockaddr_in *s = (sockaddr_in*)&addr;
		return ntohs(s->sin_port);
	}
	else if (addr.ss_family == AF_INET6) {
		sockaddr_in6 *s = (sockaddr_in6*)&addr;
		return ntohs(s->sin6_port);
	}

	return 0;
}


int TcpSocketBase::write(const uv_buf_t* buf, uint32 size)
{
	return uv_write((uv_write_t*)&m_write_t, (uv_stream_t *)getUvTcp(), buf, size, echo_write);
}

void TcpSocketBase::alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {

	TcpSocketBase* self = (TcpSocketBase*)handle->data;

	buf->base = (char *)self->mBuffer.GetBasePointer();
	buf->len = self->mBuffer.GetBufferSize();
}

void TcpSocketBase::echo_read(uv_stream_t *tcp, ssize_t nread, const uv_buf_t *buf) {

	if (nread < 0) {
		/*
		if (nread != UV_EOF)
			ERROR_LOG("Read error %s\n", uv_err_name(nread));
		*/
		uv_close((uv_handle_t*)tcp, on_uv_close);
		return;
	}

	if (nread == 0)
	{
		return;
	}

	TcpSocketBase* self = (TcpSocketBase*)tcp->data;
	self->mBuffer.WriteCompleted(nread);
	self->on_msgbuffer(&self->mBuffer);
	self->mBuffer.Reset();
}

void TcpSocketBase::on_uv_close(uv_handle_t* handle)
{
	TcpSocketBase* self = (TcpSocketBase*)handle->data;
	self->on_clsesocket();
}

void TcpSocketBase::echo_write(uv_write_t *req, int status) {
	if (status) {
		ERROR_LOG("Write error %s\n", uv_strerror(status));
	}

	/*
	write_req_t *req = (write_req_t*)malloc(sizeof(write_req_t));
	req->buf = uv_buf_init(buf->base, nread);
	uv_write((uv_write_t*)req, client, &req->buf, 1, echo_write);
	*/

	TcpSocketBase* self = (TcpSocketBase*)(req->data);
	self->on_writecomplete();
}



#include "BasePacket.h"

TcpSocket::TcpSocket(uint32 buffersize):
	TcpSocketBase(buffersize)
{
	zero();
}

TcpSocket::~TcpSocket()
{
	release();
}

void TcpSocket::write(BasePacket* packet)
{
	mSendPackets.push_back(packet);

	if (!isSending())
	{
		send_top_msg();
	}
}

BasePacket* TcpSocket::createPacket()
{
	return new BasePacket;
}

void TcpSocket::recyclePacket(BasePacket* packet)
{
	delete packet;
}

void TcpSocket::on_writecomplete()
{
	//write complete
	int size = mSending.size();
	for (int i = 0; i < size; ++i)
	{
		recyclePacket(mSendPackets.front());
		mSendPackets.pop_front();
	}
	mSending.clear();
	send_top_msg();
}

#define MAX_SEND_PACKET 10
#define MAX_SEND_BYTE 1024

bool TcpSocket::isSending()
{
	return !mSending.empty();
}

void TcpSocket::send_top_msg()
{
	int sumsize = 0;
	for (auto it = mSendPackets.begin(); it != mSendPackets.end(); ++it)
	{
		if (sumsize >= MAX_SEND_BYTE) break;
		if (mSending.size() >= MAX_SEND_PACKET) break;

		uv_buf_t tmpbuf;
		tmpbuf.base = (*it)->sendStream();
		tmpbuf.len = (*it)->sendSize();
		mSending.push_back(tmpbuf);
		sumsize += (*it)->sendSize();
	}

	if (sumsize > 0)
	{
		TcpSocketBase::write(&mSending[0], mSending.size());
	}
}

void TcpSocket::zero()
{
	_userdata = NULL;
}

void TcpSocket::release()
{
	while (!mSendPackets.empty())
	{
		recyclePacket(mSendPackets.front());
		mSendPackets.pop_front();
	}

	mSending.clear();
}