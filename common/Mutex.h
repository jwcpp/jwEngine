#pragma once

#include <atomic>

class TLock
{
public:
	explicit TLock()
	{
		flag.clear();
	}

	~TLock()
	{
	}

	void lock()
	{
		while (flag.test_and_set(std::memory_order_acquire))
			;
	}

	bool try_lock()
	{
		if (flag.test_and_set(std::memory_order_acquire))
		{
			return false;
		}

		return true;
	}

	void unlock()
	{
		flag.clear(std::memory_order_release);
	}

protected:
	mutable std::atomic_flag flag;// = ATOMIC_FLAG_INIT;

private:
	TLock& operator=(const TLock& src);
};