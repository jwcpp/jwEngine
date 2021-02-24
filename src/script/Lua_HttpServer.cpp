#include "sol/sol.hpp"

#include <functional>
#include <map>
#include <string>
#include <string_view>
#include "HttpParam.h"
#include "HttpServer.h"
#include "HttpConnect.h"
#include "EventLoop.h"

class Lua_HttpServer : public HttpServer
{
public:
	Lua_HttpServer() :
		HttpServer(EventLoop::Instance())
	{

	}
};

void luabind_httpserver(sol::state & lua)
{
	lua.new_usertype<HttpParam>("HttpParam",
		sol::constructors<HttpParam(std::string_view, char)>(),
		"find", &HttpParam::find,
		"getStr", &HttpParam::getStr,
		"getInt", &HttpParam::getInt,
		"getFloat", &HttpParam::getFloat);

	lua.new_usertype<HttpConnect>("HttpConnect", 
		sol::base_classes, sol::bases<TcpSocket>(),
		"sendMsg", &HttpConnect::sendData,
		"autoMsg", &HttpConnect::autoMsg);

	lua.new_usertype<Lua_HttpServer>("HttpServer",
		"listen", &Lua_HttpServer::listen,
		"shutdown", &Lua_HttpServer::shutdown,
		"addGet", &Lua_HttpServer::addGet,
		"addPost", &Lua_HttpServer::addPost);
}