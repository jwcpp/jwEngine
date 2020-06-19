#ifndef NET_CONNECT_H
#define NET_CONNECT_H

#include "TcpSocket.h"
#include <queue>

class NetPacket;
class NetEvent;
class NetConnect : public TcpSocket
{
public:
	NetConnect(NetEvent * nevent, uint32 buffersize = MESSAGE_BUFFER_SIZE);

	~NetConnect();

	void close(){ TcpSocket::close(); }

	// write data to socket
	void sendMsg(uint32 msgtype, NetPacket * pack);
	void sendMsg(uint32 msgtype, void * msg, uint32 len);
	void sendPacket(uint32 msgtype, NetPacket * pack) { sendMsg(msgtype, pack);}
	void sendData(uint32 msgtype, void * msg, uint32 len) { sendMsg(msgtype, msg, len);}

	static NetPacket * createPacket();
	static void recyclePacket(NetPacket * pack);
protected:

	virtual void on_msgbuffer(MessageBuffer * buffer);
	virtual void on_clsesocket();
	virtual void on_writecomplete();

private:
	void send_top_msg();
	void release();
private:
	NetPacket *  mReadPacket;
	std::queue<NetPacket *> mSendPackets;
	NetEvent * _netevent;
};

#endif