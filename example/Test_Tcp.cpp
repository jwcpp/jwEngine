#include <stdio.h>
#include <stdlib.h>


#include "NetEvent.h"
#include "EventLoop.h"
#include "NetClient.h"
#include "NetServer.h"
#include "Timer.h"
#include "NetConnect.h"
#include "NetPacket.h"

class INetEvent : public NetEvent
{
public:
	virtual void onAccept(NetConnect * conn){
		if (conn)
		{
			// 服务器通知有客户端连入
			conn->sendMsg(1, "12345", 5);
		}
	};

	virtual void onConnect(NetConnect * conn, int argv){
		if (conn)
		{
			// 客户端通知连接成功
		}
	};
	virtual void onClose(NetConnect * conn){
		
	};

	virtual void onMsg(NetConnect * conn, int msgtype, NetPacket * pack){
		std::string s;
		(*pack) >> s;

		printf("%s\n", s.c_str());
		conn->sendMsg(1, pack);
	}
};

//#define CLIENT_TEST

int main()
{
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
	EventLoop::Instance()->run();

	system("pause");
	return 0;
}