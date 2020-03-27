#include "EventLoop.h"
#include "sol/sol.hpp"

/*
class EventLoop : public Singleton<EventLoop>
{
public:
	void init();

	int Run();
	void Stop();
	void Exit();
	uv_loop_t* GetLoop();

private:
	uv_loop_t* mLoop;
};
*/


class Lua_EventLoop
{
public:
	static void init() { EventLoop::Instance()->init(); }
	static void run() { EventLoop::Instance()->Run(); }
	static void stop() { EventLoop::Instance()->Stop(); }
	static void exit() { EventLoop::Instance()->Exit(); }
};



void luabind_eventloop(sol::state & lua)
{
	//lua["eventobj"] = EventLoop::Instance();
	lua["event_init"] = &Lua_EventLoop::init;
	lua["event_run"] = &Lua_EventLoop::run;
	lua["event_stop"] = &Lua_EventLoop::stop;
	lua["event_exit"] = &Lua_EventLoop::exit;
}