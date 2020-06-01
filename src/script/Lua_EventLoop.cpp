#include "EventLoop.h"
#include "sol/sol.hpp"

/*
class EventLoop : public Singleton<EventLoop>
{
public:
	void init();

	int run();
	void stop();
	void exit();
	uv_loop_t* getLoop();

private:
	uv_loop_t* mLoop;
};
*/


class Lua_EventLoop
{
public:
	static void init() { EventLoop::Instance()->init(); }
	static void run() { EventLoop::Instance()->run(); }
	static void stop() { EventLoop::Instance()->stop(); }
	static void exit() { EventLoop::Instance()->exit(); }
};



void luabind_eventloop(sol::state & lua)
{
	//lua["eventobj"] = EventLoop::Instance();
	lua["event_init"] = &Lua_EventLoop::init;
	lua["event_run"] = &Lua_EventLoop::run;
	lua["event_stop"] = &Lua_EventLoop::stop;
	lua["event_exit"] = &Lua_EventLoop::exit;
}