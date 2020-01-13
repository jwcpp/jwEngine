#include <string>
#include <stdlib.h>

#include <Timer.h>
int main()
{
	EventLoop::Instance()->init();
	Timer t;
	t.start([&](){
		printf("1\n"); 
		t.stop();
	}, 1000, 1000);
	EventLoop::Instance()->Run();

	//system("pause");
	return 0;
}