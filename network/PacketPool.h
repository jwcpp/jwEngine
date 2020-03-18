/************************************************************************
* @file      PacketPool.h
* @brief     数据包对象池
* @author    jiangwang
* @data      2020-3-17
* @version   0.1
************************************************************************/

#pragma once

#include "BaseType.h"
#include "Objectpool.h"
#include "Singleton.h"

template<typename T>
class PacketPool : public Singleton<PacketPool<T>>
{
public:
	ObjectPool<T> objpool;
};

template<typename T>
void init_packet_pool(int count)
{
	PacketPool<T>::Instance()->objpool.init(count);
}

template<typename T>
T * create_packet()
{
	T * obj = PacketPool<T>::Instance()->objpool.createObject();
	obj->zero();
	return obj;
}

template<typename T>
void reclaim_packet(T * obj)
{
	PacketPool<T>::Instance()->objpool.reclaimObject(obj);
}

template<typename T>
void destroy_all_packet()
{
	PacketPool<T>::Instance()->objpool.destroy();
}