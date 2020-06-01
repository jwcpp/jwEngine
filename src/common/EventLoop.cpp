#include "EventLoop.h"

void EventLoop::init()
{
	mLoop = uv_default_loop();
}

int EventLoop::run()
{
	return uv_run(mLoop, UV_RUN_DEFAULT);
}

void EventLoop::stop()
{
	uv_stop(mLoop);
}

void EventLoop::exit()
{
	uv_loop_delete(mLoop);
	mLoop = NULL;
}

uv_loop_t * EventLoop::getLoop()
{
	return mLoop;
}