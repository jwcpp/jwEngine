#include "NetPacket.h"

NetPacket::NetPacket()
{
	fillPacketHead();
}

void NetPacket::zero()
{
	_rpos = 0;
	_wpos = MSG_HEAD_SIZE;
	//fillPacketHead();
	//_storage.reserve(DEFAULT_SIZE);
}

void NetPacket::writeHead()
{
	put<uint32>(MSG_LEN_POS, msglen);
	put<uint32>(MSG_TYPE_POS, msgtype);
}

uint32 NetPacket::readHead(const uint8 * p, uint32 size)
{
#if 0
	//msglen and msgtype
	memcpy(&_storage[MSG_LEN_POS], p, sizeof(uint32));
	memcpy(&_storage[MSG_TYPE_POS], p + MSG_TYPE_POS, sizeof(uint32));

	msglen = getValue<uint32>(MSG_LEN_POS);
	msgtype = getValue<uint32>(MSG_TYPE_POS);

	//start read data pos
	_rpos = MSG_HEAD_SIZE;
#endif

	int rsize = MSG_HEAD_SIZE - _rpos;
	if (rsize <= 0)
	{
		return 0;
	}

	rsize = size >= rsize ? rsize : size;
	memcpy(&_storage[_rpos], p, rsize);
	_rpos += rsize;

	if (_rpos >= MSG_HEAD_SIZE)
	{
		msglen = getValue<uint32>(MSG_LEN_POS);
		msgtype = getValue<uint32>(MSG_TYPE_POS);
	}

	return rsize;
}

void NetPacket::fillPacketHead()
{
	this->_storage.resize(MSG_HEAD_SIZE);
	_wpos = MSG_HEAD_SIZE;
}