#include "UdpPacket.h"


UdpPacket::UdpPacket()
{
	zero();
}


UdpPacket::~UdpPacket()
{
}

void UdpPacket::zero()
{
	this->_storage->resize(UDP_PACK_HEAD_SIZE);
	_wpos = UDP_PACK_HEAD_SIZE;
	_rpos = UDP_PACK_HEAD_SIZE;
}

int32  UdpPacket::getHeadSize()
{
	return UDP_PACK_HEAD_SIZE;
}

void UdpPacket::initSize(int s)
{
	if (s > size())
	{
		this->_storage->resize(s);
	}
}

int UdpPacket::getMsgType()
{
	return getValue<uint32>(0);
}

void UdpPacket::writeHead(int msgtype)
{
	put<uint32>(0, msgtype);
}

void UdpPacket::writeComplete(uint32 size)
{
	_wpos += size;
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