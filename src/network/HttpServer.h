#pragma once
#include "TcpServer.h"
#include "HttpEvent.h"
#include <map>
#include <string>
#include <functional>

class EventLoop;
class HttpServer : public TcpServer, public HttpEvent
{
public:
	HttpServer(EventLoop * loop);
	void addGet(const char * name, std::function<void(HttpConnect *, std::string_view &)> back);
	void addPost(const char * name, std::function<void(HttpConnect *, std::string_view &)> back);
protected:
	virtual TcpSocket * createSocket();
	virtual void onSocket(TcpSocket * connect);
	virtual void onClose(HttpConnect *conn);
	virtual void onMsg(HttpConnect *conn, int method, std::string_view & path, std::string_view & data);

private:
	std::map<std::string, std::function<void(HttpConnect *, std::string_view &)>> m_get;
	std::map<std::string, std::function<void(HttpConnect *, std::string_view &)>> m_post;
};

