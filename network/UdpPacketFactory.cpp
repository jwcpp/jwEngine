#include "UdpPacketFactory.h"

UdpPacketFactory * udpPacketFactory;

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