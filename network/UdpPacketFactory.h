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

#define MAX_UDP_PACKET_COUNT 500

class UdpPacketFactory
{
public:
	UdpPacketFactory(int count = MAX_UDP_PACKET_COUNT);
	
	UdpPacket * createPacket();

	void recyclePacket(UdpPacket * obj);
private:
	ObjectPool<UdpPacket> objpool;
};


extern UdpPacketFactory * udpPacketFactory;

#define CREATE_UDP_PACKET udpPacketFactory->createPacket()
#define RECYCLE_UDP_PACKET(pack) udpPacketFactory->recyclePacket(pack)
