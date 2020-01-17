#include "UdpPacketFactory.h"

UdpPacketFactory * gUdpPacketFactory = NULL;

UdpPacketFactory::UdpPacketFactory(int count) :
	objpool(count)
{

}


UdpPacket * UdpPacketFactory::createPacket()
{
	UdpPacket * packet = objpool.createObject();
	packet->zero();
	return packet;
}

void UdpPacketFactory::recyclePacket(UdpPacket * obj)
{
	if (obj)
	{
		objpool.reclaimObject(obj);
	}
}