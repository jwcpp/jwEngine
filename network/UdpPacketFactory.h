/************************************************************************
* @file      UdpPacketFactory.h
* @brief     udp数据包工厂
* @author    jiangwang
* @data      2019-12-12
* @version   0.1
************************************************************************/

#pragma once

#include "UdpPacket.h"
#include "Objectpool.h"

class UdpPacketFactory
{
public:
	UdpPacketFactory(int count);
	
	UdpPacket * createPacket();

	void recyclePacket(UdpPacket * obj);
private:
	ObjectPool<UdpPacket> objpool;
};


extern UdpPacketFactory * gUdpPacketFactory;
#define INIT_UDP_PACKET_POOL(count) gUdpPacketFactory = new UdpPacketFactory(count)
#define DESTROY_UDP_PACKET_POOL if(gUdpPacketFactory) delete gUdpPacketFactory;
#define CREATE_UDP_PACKET gUdpPacketFactory->createPacket()
#define RECYCLE_UDP_PACKET(pack) gUdpPacketFactory->recyclePacket(pack)
