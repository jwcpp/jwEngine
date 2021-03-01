#include "NetPacket.h"

NetPacket::NetPacket()
{
	zero();
}

void NetPacket::zero()
{
	this->_storage->resize(MSG_HEAD_SIZE);
	_wpos = MSG_HEAD_SIZE;
	_rpos = MSG_HEAD_SIZE;
}

int32  NetPacket::getHeadSize()
{
	return MSG_HEAD_SIZE;
}
int32  NetPacket::getMarkLen()
{
	// message head mark length
	return getValue<uint32>(MSG_LEN_POS);
}

int NetPacket::getMsgType()
{
	return getValue<uint32>(MSG_TYPE_POS);
}

bool NetPacket::isHeadFull()
{
	return wpos() >= MSG_HEAD_SIZE;
}

// send msg call
int32  NetPacket::sendSize()
{
	return wpos();
}
char * NetPacket::sendStream()
{
	return (char *)contents();
}

void NetPacket::writeHead(int msgtype)
{
	setValue<uint32>(MSG_LEN_POS, getBodySize());
	setValue<uint32>(MSG_TYPE_POS, msgtype);
}

uint32 NetPacket::readHead(const uint8 * p, uint32 size)
{
	int rsize = MSG_HEAD_SIZE - wpos();
	if (rsize <= 0)
	{
		return 0;
	}

	rsize = size >= rsize ? rsize : size;

	put(wpos(), p, rsize);
	_wpos += rsize;

	return rsize;
}