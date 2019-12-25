/************************************************************************
* @file      WsPacketFactory.h
* @brief     管理消息包对象池
* @author    jiangwang
* @data      2019-12-25
* @version   0.1
************************************************************************/

#pragma once

#include "WebSocketPacket.h"
#include "Objectpool.h"
class WsPacketFactory
{
public:
	WsPacketFactory(int count);
	~WsPacketFactory();

	WebSocketPacket * createPacket();
	void recyclePacket(WebSocketPacket * obj);

private:
	ObjectPool<WebSocketPacket> objpool;
};

extern WsPacketFactory * gWsPacketFactory;
#define INIT_WS_PACKET_POOL(count) gWsPacketFactory = new WsPacketFactory(count)
#define DESTROY_WS_PACKET_POOL if(gWsPacketFactory) delete gWsPacketFactory;
#define CREATE_WS_PACKET gWsPacketFactory->createPacket()
#define RECYCLE_WS_PACKET(pack) gWsPacketFactory->recyclePacket(pack)
