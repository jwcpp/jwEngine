#include "EventLoop.h"

void EventLoop::init()
{
	mLoop = uv_default_loop();
}

int EventLoop::Run()
{
	return uv_run(mLoop, UV_RUN_DEFAULT);
}

void EventLoop::Stop()
{
	uv_stop(mLoop);
}

void EventLoop::Exit()
{
	uv_loop_delete(mLoop);
	mLoop = NULL;
}

uv_loop_t * EventLoop::GetLoop()
{
	return mLoop;
}