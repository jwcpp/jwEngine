#include "UdpPacket.h"


UdpPacket::UdpPacket()
{
	__fillPacketHead();
}


UdpPacket::~UdpPacket()
{
}

void UdpPacket::zero()
{
	_rpos = 0;
	__fillPacketHead();
}

void UdpPacket::initSize(int s)
{
	if (s > size())
	{
		this->_storage.resize(s);
	}
}

void UdpPacket::readHead()
{
	__m_MsgType = __getValue<uint32>(0);

	//start read data pos
	_rpos = UDP_PACK_HEAD_SIZE;
}

void UdpPacket::writeHead()
{
	put<uint32>(0, __m_MsgType);
}

void UdpPacket::__fillPacketHead()
{
	this->_storage.resize(UDP_PACK_HEAD_SIZE);
	_wpos = UDP_PACK_HEAD_SIZE;
}

void UdpPacket::writeComplete(uint32 size)
{
	_wpos += size;
}