#include <stdio.h>
#include <stdlib.h>


#include "NetEvent.h"
#include "EventLoop.h"
#include "NetClient.h"
#include "NetServer.h"
#include "Timer.h"
#include "NetConnect.h"
#include "NetPacket.h"
#include "PacketPool.h"

#include "testmsg.h"
class INetEvent : public NetEvent
{
public:
	virtual void onAccept(NetConnect * conn){
		if (conn)
		{
			testmsg msg;
			msg.id = 10000;
			msg.arr = { 11, 22, 33, 44, 55 };
			msg.play = {"world", 80, 3};
			msg.attrs.push_back(attr{ 111, 222 });
			msg.attrs.push_back(attr{ 333, 444 });

			NetPacket packet;
			msg.write(&packet);

			conn->sendMsg(1, &packet);
		}
	};

	virtual void onConnect(NetConnect * conn){
		if (conn)
		{

		}
	};
	virtual void onClose(NetConnect * conn){

	};

	virtual void onMsg(NetConnect * conn, NetPacket * pack){
		testmsg msg;
		msg.read(pack);

		printf("msg id: %d\n", msg.id);
		printf("msg.arr: ");
		for (int i = 0; i < msg.arr.size(); ++i)
		{
			printf("%d  ", msg.arr[i]);
		}

		printf("player name: %s\n", msg.play.name.c_str());
		printf("player level: %d\n", msg.play.level);
		printf("player msgcount: %d\n", msg.play.msgcount);

		printf(" msg.attrs\n");
		for (int i = 0; i < msg.attrs.size(); ++ i)
		{
			printf("msg.attrs[%d].attack: %d \n", i, msg.attrs[i].attack);
			printf("msg.attrs[%d].hp: %d \n", i, msg.attrs[i].hp);
		}
	}
};

//#define CLIENT_TEST

int main()
{
	//初始化tcp包内存池
	init_packet_pool<NetPacket>(10);

	//初始化事件循环
	INetEvent eve;
	EventLoop::Instance()->init();

#ifdef CLIENT_TEST
	NetClient client(EventLoop::Instance(), &eve);
	client.connect("127.0.0.1", 3001);

#else
	NetServer server(EventLoop::Instance(), &eve);
	server.listen("127.0.0.1", 3001);

#endif

	//开启事件循环
	EventLoop::Instance()->Run();

	system("pause");
	return 0;
}