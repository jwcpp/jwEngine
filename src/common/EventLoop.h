#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include "libuv/include/uv.h"
#include "Singleton.h"

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


#endif