#include "TcpPacketFactory.h"

TPacketFactory * gTcpPacketFactory = NULL;

TPacketFactory::TPacketFactory(int count) :
	objpool(count)
{

}

NetPacket * TPacketFactory::createPacket()
{
	NetPacket * packet = objpool.createObject();
	packet->zero();
	return packet;
}

void TPacketFactory::recyclePacket(NetPacket * obj)
{
	if (obj)
	{
		objpool.reclaimObject(obj);
	}
}