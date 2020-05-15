#ifndef MSG_PACKET_H
#define MSG_PACKET_H

#include "BasePacket.h"
#include "PoolObject.h"

#define MSG_LEN_POS  0
#define MSG_TYPE_POS sizeof(uint32)
#define MSG_HEAD_SIZE (sizeof(uint32) + sizeof(uint32))

// packet head( msglen:uint32, msgtype:uint32 )
class NetPacket : public BasePacket
{

public:
	NetPacket();

	virtual int32  getBodySize();
	virtual char * getBodyData();

	// read msg call
	virtual int32  getHeadSize();
	virtual int32  getMarkLen();   // message head mark length
	virtual int getMsgType();
	virtual bool isHeadFull();

	// send msg call
	virtual int32  sendSize();
	virtual char * sendStream();

	void writeHead(int msgtype);
	uint32 readHead(const uint8 * p, uint32 size);
private:
	
	virtual void _fillHead();

	INCLUDE_POOL_OBJECT
};

#endif