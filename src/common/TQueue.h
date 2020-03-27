#ifndef X_QUEUE_H
#define X_QUEUE_H

#include "concurrentqueue/concurrentqueue.h"

/*

using namespace moodycamel;

ReaderWriterQueue<int> q(100);       // Reserve space for at least 100 elements up front

q.enqueue(17);                       // Will allocate memory if the queue is full
bool succeeded = q.try_enqueue(18);  // Will only succeed if the queue has an empty slot (never allocates)
assert(succeeded);

int number;
succeeded = q.try_dequeue(number);  // Returns false if the queue was empty

assert(succeeded && number == 17);

// You can also peek at the front item of the queue (consumer only)
int* front = q.peek();
assert(*front == 18);
succeeded = q.try_dequeue(number);
assert(succeeded && number == 18);
front = q.peek();
assert(front == nullptr);           // Returns nullptr if the queue was empty

*/

template<typename T>
class TQueue
{
public:
	TQueue(int maxSize = 15) :
		mqueue(maxSize)
	{
	
	}
	void push(const T& object)
	{
		mqueue.enqueue(object);
	}

	bool tryPop(T& object)
	{
		return mqueue.try_dequeue(object);
	}

private:
	moodycamel::ConcurrentQueue<T> mqueue;
};

#endif