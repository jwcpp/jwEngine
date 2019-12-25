#pragma once
//#include "Singleton.h"
#include "NetPacket.h"
#include "Objectpool.h"

//tcp packet factory
class TPacketFactory
{
public:
	TPacketFactory(int count);

	NetPacket * createPacket();

	void recyclePacket(NetPacket * obj);

private:
	ObjectPool<NetPacket> objpool;
};

extern TPacketFactory * gTcpPacketFactory;
#define INIT_TCP_PACKET_POOL(count) gTcpPacketFactory = new TPacketFactory(count)
#define DESTROY_TCP_PACKET_POOL if(gTcpPacketFactory) delete gTcpPacketFactory;
#define CREATE_TCP_PACKET gTcpPacketFactory->createPacket()
#define RECYCLE_TCP_PACKET(pack) gTcpPacketFactory->recyclePacket(pack)