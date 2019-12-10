#ifndef NET_EVENT_H
#define NET_EVENT_H

class NetConnect;
class NetPacket;

class NetEvent
{
public:
	virtual NetConnect * createConnect();
	virtual void destroyConnect(NetConnect * conn);

	virtual void onAccept(NetConnect * conn){};
	virtual void onConnect(NetConnect * conn){};
	virtual void onClose(NetConnect * conn){};
	virtual void onMsg(NetConnect * conn, NetPacket * pack){};

};

#endif