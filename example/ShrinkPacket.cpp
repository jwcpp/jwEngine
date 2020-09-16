#include "CommonPool.h"
#include "UTimer.h"
#include "BasePacket.h"

int main()
{
	CommPool::init<BasePacket>(10);

	// test
	BasePacket * p1 = CommPool::create<BasePacket>();
	BasePacket* p2 = CommPool::create<BasePacket>();
	p1->resize(65535 + 1);
	CommPool::reclaim(p1);
	CommPool::reclaim(p2);

	EventLoop::Instance()->init();
	
	UTimer ut;
	ut.start([]() {
		std::list<BasePacket*> & tlist = CommPool::getobjs<BasePacket>();
		for (auto it = tlist.begin(); it != tlist.end(); ++it)
		{
			(*it)->shrink(65535);
		}
	
	}, 1000 * 60 * 30, 1000 * 60 * 30);


	EventLoop::Instance()->run();
	return 0;
}