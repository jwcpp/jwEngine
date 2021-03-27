#include <stdio.h>
#include <map>
#include <string_view>
#include <functional>
#include "HttpServer.h"
#include "EventLoop.h"
#include "HttpConnect.h"
#include "HttpParser.h"
#include <iostream>

class HttpExt
{
public:
	void printField(const char* p, int len)
	{
		std::string_view v(p, len);
		std::cout << v <<": ";
	}

	void printValue(const char* p, int len)
	{
		std::string_view v(p, len);
		std::cout << v << std::endl;
	}
};

HttpExt httpext;

int main()
{
	EventLoop::Instance()->init();
	HttpServer server(EventLoop::Instance());
	server.listen("127.0.0.1", 80);

	server.addGet("/", [](HttpConnect* conn, std::string_view& data) {
		conn->autoMsg("hello world");
		});

	HttpSettings* setting = getHttpSettings();
	setting->on_message_begin = [](llhttp_t* http)->int {
		http->data = &httpext;
		return 0;
	};

	setting->on_header_field = [](llhttp_t* http, const char* at, size_t length)->int {
		HttpExt* ext = (HttpExt*)(http->data);
		ext->printField(at, length);
		return 0;
	};

	setting->on_header_value = [](llhttp_t* http, const char* at, size_t length)->int {
		HttpExt* ext = (HttpExt*)(http->data);
		ext->printValue(at, length);
		return 0;
	};

	EventLoop::Instance()->run();
	return 0;
}