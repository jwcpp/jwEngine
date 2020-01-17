#include <string>
#include <stdlib.h>

#include <Timer.h>
int main()
{
	EventLoop::Instance()->init();
	Timer * t = new Timer;
	t->start([&](){
		printf("1\n"); 
		t->stop();

		delete t;

	}, 1000, 1000);

	EventLoop::Instance()->Run();

	//system("pause");
	return 0;
}