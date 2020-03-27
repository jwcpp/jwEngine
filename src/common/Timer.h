#pragma once

#include "Heap.h"
#include "BaseType.h"
#include <functional>

class Timer
{
	struct TimerNode
	{
		struct Heap::heap_node m_node;
		void * user = NULL;
	};

	//typedef void(*close_timer_cb)(TimerNode* timernode);

public:
	Timer();
	~Timer();

	static void init();
	static void update();

	void start(std::function<void()> cb, uint64_t timeout, uint64_t repeat = 0);
	void stop();

private:
	static int compare_fn(const struct Heap::heap_node* a, const struct Heap::heap_node* b);

private:
	TimerNode * m_node = NULL;
	uint32 m_time = 0;
	uint32 m_repeat = 0;
	std::function<void()> m_cb;

	static struct Heap::heap gRootHeap;
};

