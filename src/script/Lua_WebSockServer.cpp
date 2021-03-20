#include <string>
#include "sol/sol.hpp"

#include "WebSocketEvent.h"
#include "WebSocketServer.h"
#include "WebSocketPacket.h"
#include "WebSocketConnect.h"
#include "EventLoop.h"

class Lua_WebSocketServer : public WebSocketServer, public WebSocketEvent
{
public:
	Lua_WebSocketServer() :
		WebSocketServer(EventLoop::Instance(), this)
	{

	}

protected:


	virtual void onAccept(WebSocketConnect * conn) {
		on_accept(conn);
	};
	virtual void onClose(WebSocketConnect * conn) {
		on_close(conn);
	};
	virtual void onMsg(WebSocketConnect * conn, WebSocketPacket * pack) {
		on_msg(conn, pack);
	};

public:
	std::function<void(WebSocketConnect *)> on_accept;
	std::function<void(WebSocketConnect *)> on_close;
	std::function<void(WebSocketConnect *, WebSocketPacket *)> on_msg;
};

void luabind_websocket(sol::state & lua)
{
	lua.new_usertype<WebSocketConnect>("WebSocketConnect",
		sol::base_classes, sol::bases<TcpSocket>(),
		"sendPacket", &WebSocketConnect::sendPacket,
		"sendMsg", &WebSocketConnect::sendData);

	lua.new_usertype<Lua_WebSocketServer>("WebSocketServer",
		"listen", &Lua_WebSocketServer::listen,
		"shutdown", &Lua_WebSocketServer::shutdown,
		"on_accept", &Lua_WebSocketServer::on_accept,
		"on_close", &Lua_WebSocketServer::on_close,
		"on_msg", &Lua_WebSocketServer::on_msg);

	lua.new_usertype<WebSocketPacket>("WebSocketPacket",
		sol::base_classes, sol::bases<BasePacket>());
}