#include "UdpPacketFactory.h"

UdpPacketFactory * gUdpPacketFactory = NULL;

UdpPacketFactory::UdpPacketFactory(int count) :
	objpool(count)
{

}


UdpPacket * UdpPacketFactory::createPacket()
{
	return objpool.createObject();
}

void UdpPacketFactory::recyclePacket(UdpPacket * obj)
{
	objpool.reclaimObject(obj);
}