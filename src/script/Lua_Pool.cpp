#include "sol/sol.hpp"
#include "CommonPool.h"
#include "UdpPacket.h"
#include "NetPacket.h"
#include "WebSocketPacket.h"

void init_basepacket_pool(int count)
{
	CommPool::init<BasePacket>(count);
}

void init_kcppacket_pool(int count)
{
	CommPool::init<UdpPacket>(count);
}

void init_netpacket_pool(int count)
{
	CommPool::init<NetPacket>(count);
}

void init_wspacket_pool(int count)
{
	CommPool::init<WebSocketPacket>(count);
}

class LuaPacket {
public:
	LuaPacket():m_type(0), obj(NULL){

	}

	BasePacket* create_base_pak() {
		if (!obj){
			m_type = 1;
			obj = CommPool::create<BasePacket>();
		}
		return obj;
	}
	UdpPacket* create_kcp_pak() {
		if (!obj){
			m_type = 2;
			obj = CommPool::create<UdpPacket>();
		}
		return static_cast<UdpPacket*>(obj);
	}

	NetPacket* create_net_pak() {
		if (!obj) {
			m_type = 3;
			obj = CommPool::create<NetPacket>();
		}
		return static_cast<NetPacket*>(obj);
	}

	WebSocketPacket* create_ws_pak() {
		if (!obj) {
			m_type = 4;
			obj = CommPool::create<WebSocketPacket>();
		}
		return static_cast<WebSocketPacket*>(obj);
	}

	~LuaPacket()
	{
		if (!obj) return;

		switch (m_type)
		{
		case 1:
			CommPool::reclaim(obj);
			break;
		case 2:
			CommPool::reclaim(static_cast<UdpPacket*>(obj));
			break;
		case 3:
			CommPool::reclaim(static_cast<NetPacket*>(obj));
			break;
		case 4:
			CommPool::reclaim(static_cast<WebSocketPacket*>(obj));
			break;
		default:
			break;
		}
		
	}

private:
	int m_type;
	BasePacket* obj;
};

void luabind_pool(sol::state& lua)
{
	lua["init_basepacket_pool"] = &init_basepacket_pool;
	lua["init_kcppacket_pool"] = &init_kcppacket_pool;
	lua["init_netpacket_pool"] = &init_netpacket_pool;
	lua["init_wspacket_pool"] = &init_wspacket_pool;

	lua.new_usertype<LuaPacket>("AutoPacket",
		"create_base_pak", &LuaPacket::create_base_pak,
		"create_kcp_pak", &LuaPacket::create_kcp_pak,
		"create_net_pak", &LuaPacket::create_net_pak, 
		"create_ws_pak", &LuaPacket::create_ws_pak);
}


// c++
/*
	AutoPacket<NetPacket> apk;
	NetPacket * pack = *apk;
	(*pack)<<1314;
	(*pack)<<"jw";
	connect->sendMsg(msgtype, pack);
*/

// lua
/*
	ap = AutoPacket:new()
	packet = ap:create_net_pak()
	packet:pushInt32(123)
*/