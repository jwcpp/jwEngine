#include <string>
#include <stdlib.h>

#include "BaseType.h"
#include "Timer.h"
#include "Tools.h"



int main()
{
	Timer::init();


	Timer timer1;
	Timer timer2;

	timer1.start([](){
		printf("--->30\n");
	},30 * 1000);

	timer2.start([](){
		printf("--->10\n");
	}, 10 * 1000, 10 * 1000);

	Timer * timer3 = new Timer;
	timer3->start([&](){
		printf("--->20\n");

		timer3->stop();
		delete timer3;


	}, 20 * 1000, 1000);


	while (1)
	{
		Tools::sleep(10);
		Timer::update();
	}

	system("pause");
	return 0;
}