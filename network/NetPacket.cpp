#include "NetPacket.h"

NetPacket::NetPacket()
{
	fillPacketHead();
}

void NetPacket::clear()
{
	_rpos = 0;
	fillPacketHead();
	_storage.reserve(DEFAULT_SIZE);
}

void NetPacket::writeHead()
{
	put<uint32>(MSG_LEN_POS, msglen);
	put<uint32>(MSG_TYPE_POS, msgtype);
}

void NetPacket::readHead(const uint8 * p)
{
	//msglen and msgtype
	memcpy(&_storage[MSG_LEN_POS], p, sizeof(uint32));
	memcpy(&_storage[MSG_TYPE_POS], p + MSG_TYPE_POS, sizeof(uint32));

	msglen = getValue<uint32>(MSG_LEN_POS);
	msgtype = getValue<uint32>(MSG_TYPE_POS);

	//start read data pos
	_rpos = MSG_HEAD_SIZE;
}

void NetPacket::fillPacketHead()
{
	this->_storage.resize(MSG_HEAD_SIZE);
	_wpos = MSG_HEAD_SIZE;
}