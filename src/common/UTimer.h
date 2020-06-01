#ifndef UV_TIMER_H
#define UV_TIMER_H

#include "EventLoop.h"
#include <functional>

class UTimer
{
public:
	UTimer()
	{
		m_timer = (uv_timer_t *)malloc(sizeof(uv_timer_t));
		m_timer->data = this;
		uv_timer_init(EventLoop::Instance()->getLoop(), m_timer);
	}

	~UTimer()
	{
		uv_close((uv_handle_t *)m_timer, [](uv_handle_t* handle){
			free(handle);
		});
	}

	void start(std::function<void()> cb, uint64_t timeout, uint64_t repeat = 0)
	{
		m_cb = cb;
		uv_timer_start(m_timer, timer_cb, timeout, repeat);
	}

	void stop()
	{
		uv_timer_stop(m_timer);
	}

private:
	
	static void timer_cb(uv_timer_t* handle)
	{
		UTimer * timer = (UTimer *)handle->data;
		timer->m_cb();
	}

private:
	uv_timer_t * m_timer;
	std::function<void()> m_cb;

	/*
		uv_timer_t m_timer; 
		£¨This is the wrong approach£©

		class Test
		{
			void go()
			{
				timer.start([&]()
				{
					delete this; // m_timer Object is released
				}1000);
			}
			
			Timer timer;
		}
	*/
};

#endif