#pragma once
//#include "Singleton.h"
#include "NetPacket.h"
#include "Objectpool.h"

#define MAX_TCP_PACKET_COUNT 500

//tcp packet factory
class TPacketFactory
{
public:
	TPacketFactory(int count = MAX_TCP_PACKET_COUNT);

	NetPacket * createPacket();

	void recyclePacket(NetPacket * obj);

private:
	ObjectPool<NetPacket> objpool;
};

extern TPacketFactory * tcpPacketFactory;

#define CREATE_TCP_PACKET tcpPacketFactory->createPacket()
#define RECYCLE_TCP_PACKET(pack) tcpPacketFactory->recyclePacket(pack)