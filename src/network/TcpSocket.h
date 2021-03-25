#ifndef CONNECT_H
#define CONNECT_H

#define SOCKET_BUFFER_SIZE 65536

#include "BaseType.h"
#include "libuv/include/uv.h"
#include "MessageBuffer.h"
#include <string>


class TcpSocketBase
{
public:
	TcpSocketBase(uint32 buffersize = MESSAGE_BUFFER_SIZE);
	virtual ~TcpSocketBase();

	int init_uv_tcp(uv_loop_t * loop){ return uv_tcp_init(loop, getUvTcp()); }
	int accept(uv_stream_t *server){ return uv_accept(server, (uv_stream_t*)getUvTcp()); }

	void clear();
	void close();
	void on_read_start();
	int write(const uv_buf_t * buf, uint32 size);

	//static void createConnect(uv_loop_t * loop, const char * ip, int port, std::function<void(std::shared_ptr<TcpSocket> &)> cb, bool ipv6 = false);

	std::string localIP() const;
	int localPort() const;
	std::string remoteIP() const;
	int remotePort() const;
	uv_tcp_t* getUvTcp() { return &m_uv_tcp; }

protected:

	virtual void on_msgbuffer(MessageBuffer * buffer){};
	virtual void on_clsesocket(){};
	virtual void on_writecomplete(){};

private:

	static void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
	static void echo_read(uv_stream_t *tcp, ssize_t nread, const uv_buf_t *buf);
	static void on_uv_close(uv_handle_t* handle);
	static void echo_write(uv_write_t *req, int status);

private:
	uv_tcp_t m_uv_tcp;
	uv_write_t m_write_t;
	MessageBuffer mBuffer;
};

#include <list>
#include <vector>
class BasePacket;
class TcpSocket : public TcpSocketBase
{
public:
	TcpSocket(uint32 buffersize = MESSAGE_BUFFER_SIZE);
	~TcpSocket();

	virtual void zero();
	virtual void release();

	void setUserdata(void* userdata) { _userdata = userdata; }
	void* getUserdata() { return _userdata; }
	void setUsernum(uint32 num) { _userdata = (void*)num; }
	uint32 getUsernum() { return (uint32)_userdata; }
protected:

	void write(BasePacket * packet);
	
	BasePacket* createPacket();
	virtual void recyclePacket(BasePacket * packet);
	virtual void on_writecomplete();
private:
	bool isSending();
	void send_top_msg();
private:
	std::vector<uv_buf_t> mSending;
	std::list<BasePacket*> mSendPackets;

	void* _userdata;
};


#endif