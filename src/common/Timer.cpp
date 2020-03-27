#include "Timer.h"
#include "XTime.h"

struct Heap::heap Timer::gRootHeap;

Timer::Timer()
{
	m_node = (TimerNode *)malloc(sizeof(TimerNode));
	m_node->user = this;
}

Timer::~Timer()
{
	if (m_node)
	{
		m_node->user = NULL;
		
		stop();
		free(m_node);

		m_node = NULL;
	}
}


/*static */ void Timer::init()
{
	Heap::CHeap::heap_init(&gRootHeap);
}
/*static */ void Timer::update()
{
	for (int i = 0; i < 10; ++i)
	{
		TimerNode * node = (TimerNode *)Heap::CHeap::heap_min(&gRootHeap);
		if (node)
		{
			Timer * timer = (Timer *)node->user;
			if (XTime::iclock() >= timer->m_time)
			{
				Heap::CHeap::heap_remove(&gRootHeap, (struct Heap::heap_node *)node, (Heap::heap_compare_fn)compare_fn);
				timer->m_time = 0;
				if (timer->m_repeat > 0)
				{
					timer->m_time = timer->m_repeat + XTime::iclock();
					Heap::CHeap::heap_insert(&gRootHeap, (struct Heap::heap_node *)node, (Heap::heap_compare_fn)compare_fn);
				}

				timer->m_cb();
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
}

void Timer::start(std::function<void()> cb, uint64_t timeout, uint64_t repeat)
{
	m_cb = cb;
	m_time = timeout + XTime::iclock();
	m_repeat = repeat;
	Heap::CHeap::heap_insert(&gRootHeap, (struct Heap::heap_node *)m_node, (Heap::heap_compare_fn)compare_fn);
}
void Timer::stop()
{
	if (m_time == 0)
		return;

	m_time = 0;
	Heap::CHeap::heap_remove(&gRootHeap, (struct Heap::heap_node *)m_node, (Heap::heap_compare_fn)compare_fn);
}

int Timer::compare_fn(const struct Heap::heap_node* a, const struct Heap::heap_node* b)
{
	TimerNode * node_a = (TimerNode *)a;
	TimerNode * node_b = (TimerNode *)b;

	Timer * timer_a = (Timer *)node_a->user;
	Timer * timer_b = (Timer *)node_b->user;

	if (timer_a->m_time < timer_b->m_time)
		return 1;
	else if (timer_b->m_time < timer_a->m_time)
		return 0;

	/*
	  if (a->start_id < b->start_id)
	  return 1;
	  if (b->start_id < a->start_id)
	  return 0;
	*/

	return 0;
}