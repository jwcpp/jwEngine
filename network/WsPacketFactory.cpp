#include "WsPacketFactory.h"


WsPacketFactory * gWsPacketFactory = NULL;
WsPacketFactory::WsPacketFactory(int count) :
	objpool(count)
{
}


WsPacketFactory::~WsPacketFactory()
{
}


WebSocketPacket * WsPacketFactory::createPacket()
{
	WebSocketPacket * packet = objpool.createObject();
	packet->zero();
	return packet;
}
void WsPacketFactory::recyclePacket(WebSocketPacket * obj)
{
	if (obj)
	{
		objpool.reclaimObject(obj);
	}
}