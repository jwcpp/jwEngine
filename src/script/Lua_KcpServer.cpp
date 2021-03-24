#include <string>
#include "sol/sol.hpp"

#include "KcpEvent.h"
#include "KcpClient.h"
#include "KcpServer.h"
#include "UdpPacket.h"
#include "KcpSession.h"
#include "EventLoop.h"

class Lua_KcpServer : public KcpServer, public KcpEvent
{
public:
	Lua_KcpServer() :
		KcpServer(EventLoop::Instance(), this)
	{

	}

protected:


	virtual void onAccept(KcpSession * conn) {
		on_accept(conn);
	};
	virtual void onClose(KcpSession * conn) {
		on_close(conn);
	};
	virtual void onMsg(KcpSession * conn, int msgtype, UdpPacket * pack) {
		on_msg(conn, msgtype, pack);
	};

public:
	std::function<void(KcpSession *)> on_accept;
	std::function<void(KcpSession *)> on_close;
	std::function<void(KcpSession *,int, UdpPacket *)> on_msg;
};

class Lua_KcpClient : public KcpClient, public KcpEvent
{
public:
	Lua_KcpClient() :
		KcpClient(EventLoop::Instance(), this)
	{

	}

protected:

	virtual void onConnect(KcpSession * conn) {
		on_connect(conn);
	};
	virtual void onClose(KcpSession * conn) {
		on_close(conn);
	};
	virtual void onMsg(KcpSession * conn, int msgtype, UdpPacket * pack) {
		on_msg(conn, msgtype, pack);
	};

public:
	std::function<void(KcpSession *)> on_connect;
	std::function<void(KcpSession *)> on_close;
	std::function<void(KcpSession *, int, UdpPacket *)> on_msg;
};


void luabind_kcpserver(sol::state & lua)
{

	lua.new_usertype<KcpSession>("KcpSession",
		"sendPacket", &KcpSession::sendPacket,
		"sendMsg", &KcpSession::sendData,
		"flushKcp", &KcpSession::flushKcp,
		"getSessionId", &KcpSession::getSessionId);

	lua.new_usertype<Lua_KcpClient>("KcpClient",
		//sol::constructors<Lua_NetClient(EventLoop *)>(),
		"createSession", &Lua_KcpClient::createSession,
		"closeSession", &Lua_KcpClient::closeSession,
		"on_connect", &Lua_KcpClient::on_connect,
		"on_close", &Lua_KcpClient::on_close,
		"on_msg", &Lua_KcpClient::on_msg);

	lua.new_usertype<Lua_KcpServer>("KcpServer",
		//sol::constructors<Lua_NetServer(EventLoop *)>(),
		"start", &Lua_KcpServer::start,
		"closeSession", &Lua_KcpServer::closeSession,
		"shutdown", &Lua_KcpServer::shutdown,
		"on_accept", &Lua_KcpServer::on_accept,
		"on_close", &Lua_KcpServer::on_close,
		"on_msg", &Lua_KcpServer::on_msg);

	lua.new_usertype<UdpPacket>("KcpPacket",
		sol::base_classes, sol::bases<BasePacket>());
}