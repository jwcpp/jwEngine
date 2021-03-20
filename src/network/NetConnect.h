#ifndef NET_CONNECT_H
#define NET_CONNECT_H

#include "TcpSocket.h"

class NetPacket;
class NetEvent;
class NetConnect : public TcpSocket
{
public:
	NetConnect(NetEvent * nevent, uint32 buffersize = MESSAGE_BUFFER_SIZE);

	~NetConnect();

	// write data to socket
	void sendMsg(uint32 msgtype, NetPacket * pack);
	void sendMsg(uint32 msgtype, const char * msg, uint32 len);
	// lua call
	void sendPacket(uint32 msgtype, NetPacket * pack) { sendMsg(msgtype, pack);}
	void sendData(uint32 msgtype, std::string_view view) { sendMsg(msgtype, view.data(), view.size()); }

protected:
	NetPacket* createPacket();
	//virtual void recyclePacket(BasePacket* pack);

	virtual void on_msgbuffer(MessageBuffer * buffer);
	virtual void on_clsesocket();
private:
	NetPacket *  mReadPacket;
	NetEvent * _netevent;
};

#endif