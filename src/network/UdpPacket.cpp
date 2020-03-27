#include "UdpPacket.h"


UdpPacket::UdpPacket()
{
	_fillHead();
}


UdpPacket::~UdpPacket()
{
}

void UdpPacket::initSize(int s)
{
	if (s > size())
	{
		this->_storage.resize(s);
	}
}

uint32 UdpPacket::getMsgType()
{
	return getValue<uint32>(0);
}
void UdpPacket::readHead()
{
	//start read data pos
	_rpos = UDP_PACK_HEAD_SIZE;
}

void UdpPacket::writeHead(int msgtype)
{
	put<uint32>(0, msgtype);
}

void UdpPacket::_fillHead()
{
	this->_storage.resize(UDP_PACK_HEAD_SIZE);
	_wpos = UDP_PACK_HEAD_SIZE;
}

void UdpPacket::writeComplete(uint32 size)
{
	_wpos += size;
}

int32  UdpPacket::getBodySize()
{
	return wpos() - UDP_PACK_HEAD_SIZE;
}

char * UdpPacket::getBodyData()
{
	return (char *)(contents() + UDP_PACK_HEAD_SIZE);
}

// read msg call
int32  UdpPacket::getHeadSize()
{
	return UDP_PACK_HEAD_SIZE;
}

int32  UdpPacket::getMarkLen()
{
	// udp There is no need to mark the length
	return 0;
}

// send msg call
int32  UdpPacket::sendSize()
{
	return wpos();
}

char * UdpPacket::sendStream()
{
	return (char *)contents();
}