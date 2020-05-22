#include "sol/sol.hpp"

#include <map>
#include "HttpParam.h"
#include "HttpServer.h"
#include "HttpConnect.h"
#include "EventLoop.h"
#include <functional>
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
		sol::constructors<HttpParam(std::string_view &, char)>(),
		"find", &HttpParam::find,
		"getStr", &HttpParam::getStr,
		"getInt", &HttpParam::getInt,
		"getFloat", &HttpParam::getFloat);

	lua.new_usertype<HttpConnect>("HttpConnect",
		"sendMsg", &HttpConnect::sendMsg,
		"autoMsg", &HttpConnect::autoMsg);

	lua.new_usertype<Lua_HttpServer>("HttpServer",
		"listen", &Lua_HttpServer::listen,
		"shutdown", &Lua_HttpServer::shutdown,
		"addGet", &Lua_HttpServer::addGet,
		"addPost", &Lua_HttpServer::addPost);
}