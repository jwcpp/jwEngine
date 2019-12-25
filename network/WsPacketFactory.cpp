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
	return objpool.createObject();
}
void WsPacketFactory::recyclePacket(WebSocketPacket * obj)
{
	objpool.reclaimObject(obj);
}