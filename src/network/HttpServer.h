#pragma once
#include "TcpServer.h"
#include "HttpEvent.h"

class EventLoop;
class HttpServer : public TcpServer, public HttpEvent
{
public:
	HttpServer(EventLoop * loop);
	void addGet(const char * name, std::function<void(HttpConnect *, std::string_view &)> back);
	void addPost(const char * name, std::function<void(HttpConnect *, std::string_view &)> back);
	void setOther(std::function<void(HttpConnect*, HttpParser*)> back);
protected:
	virtual TcpSocket * createSocket();
	virtual void onSocket(TcpSocket * connect);
	virtual void onClose(HttpConnect *conn);
	virtual void onGet(HttpConnect* conn, std::string_view& path, std::string_view& data);
	virtual void onPost(HttpConnect* conn, std::string_view& path, std::string_view& data);
	virtual void onOther(HttpConnect* conn, HttpParser* parser);

private:
	std::map<std::string, std::function<void(HttpConnect *, std::string_view &)>> m_get;
	std::map<std::string, std::function<void(HttpConnect *, std::string_view &)>> m_post;
	std::function<void(HttpConnect*, HttpParser*)> m_other;
};

