#ifndef MSG_PACKET_H
#define MSG_PACKET_H

#include "ByteBuffer.h"
#include "PoolObject.h"

#define MSG_LEN_POS  0
#define MSG_TYPE_POS sizeof(uint32)
#define MSG_HEAD_SIZE (sizeof(uint32) + sizeof(uint32))

// packet head( msglen:uint32, msgtype:uint32 )
class NetPacket : public ByteBuffer
{

public:
	NetPacket();

	void clear();

	void setMsgLen(uint32 len){ msglen = len; }
	uint32 getMsgLen(){ return msglen; }
	void setMsgType(uint32 type){ msgtype = type; }
	uint32 getMsgType(){ return msgtype;}

	const uint8 * getData(){ return _storage.data(); }
	const uint8 * getBodyData(){ return _storage.data() + MSG_HEAD_SIZE; }

	void writeHead();
	void readHead(const uint8 * p);
private:
	
	void fillPacketHead();

	template<typename T>
	T getValue(uint32 pos){
		T value;
		std::memcpy(&value, &_storage[pos], sizeof(T));
		EndianConvert(value);

		return value;
	}

private:
	uint32 msglen;
	uint32 msgtype;
	INCLUDE_POOL_OBJECT
};

#endif