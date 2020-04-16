#include <string>
#include "sol/sol.hpp"

#include "NetEvent.h"
#include "NetClient.h"
#include "NetServer.h"
#include "NetPacket.h"
#include "NetConnect.h"
#include "EventLoop.h"
#include "PacketPool.h"

class Lua_NetServer : public NetServer, public NetEvent
{
public:
	Lua_NetServer() :
		NetServer(EventLoop::Instance(), this)
	{

	}

protected:


	virtual void onAccept(NetConnect * conn) {
		on_accept(conn);
	};
	virtual void onClose(NetConnect * conn) {
		on_close(conn);
	};
	virtual void onMsg(NetConnect * conn, NetPacket * pack) {
		on_msg(conn, pack);
	};

public:
	std::function<void(NetConnect * )> on_accept;
	std::function<void(NetConnect * )> on_close;
	std::function<void(NetConnect * , NetPacket *)> on_msg;
};

class Lua_NetClient : public NetClient, public NetEvent
{
public:
	Lua_NetClient():
		NetClient(EventLoop::Instance(), this)
	{
	
	}

protected:

	virtual void onConnect(NetConnect * conn, int argv) {
		on_connect(conn, argv);
	};
	virtual void onClose(NetConnect * conn) {
		on_close(conn);
	};
	virtual void onMsg(NetConnect * conn, NetPacket * pack) {
		on_msg(conn, pack);
	};

public:
	std::function<void(NetConnect *, int)> on_connect;
	std::function<void(NetConnect *)> on_close;
	std::function<void(NetConnect *, NetPacket *)> on_msg;
};

void init_netpacket_pool(int count)
{
	init_packet_pool<NetPacket>(count);
}


void luabind_netserver(sol::state & lua)
{
	lua["init_netpacket_pool"] = &init_netpacket_pool;

	lua.new_usertype<NetConnect>("NetConnect",
		"close", &NetConnect::close,
		"sendPacket", &NetConnect::sendPacket,
		"sendData", &NetConnect::sendData);

	lua.new_usertype<Lua_NetClient>("NetClient",
		//sol::constructors<Lua_NetClient(EventLoop *)>(),
		"connect", &Lua_NetClient::connect,
		"on_connect", &Lua_NetClient::on_connect,
		"on_close", &Lua_NetClient::on_close,
		"on_msg", &Lua_NetClient::on_msg);

	lua.new_usertype<Lua_NetServer>("NetServer",
		//sol::constructors<Lua_NetServer(EventLoop *)>(),
		"listen", &Lua_NetServer::listen,
		"shutdown", &Lua_NetServer::shutdown,
		"on_accept", &Lua_NetServer::on_accept,
		"on_close", &Lua_NetServer::on_close,
		"on_msg", &Lua_NetServer::on_msg);

	lua.new_usertype<NetPacket>("NetPacket",
		sol::base_classes, sol::bases<BasePacket>());
}