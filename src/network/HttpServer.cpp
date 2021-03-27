#include <map>
#include <functional>
#include <string_view>
#include "HttpServer.h"
#include "HttpConnect.h"
#include "CommonPool.h"

HttpServer::HttpServer(EventLoop * loop):
	TcpServer(loop)
{

}

TcpSocket * HttpServer::createSocket()
{
	HttpConnect * conn = CommPool::create<HttpConnect>();
	conn->setEvent(this);
	return conn;
}

void HttpServer::onSocket(TcpSocket * connect)
{

}

void HttpServer::onClose(HttpConnect *conn)
{
	CommPool::reclaim(conn);
}

void HttpServer::onGet(HttpConnect* conn, std::string_view& path, std::string_view& data)
{
	std::string spath(path);
	auto it = m_get.find(spath);
	if (it != m_get.end())
	{
		(it->second)(conn, data);
	}
}

void HttpServer::onPost(HttpConnect* conn, std::string_view& path, std::string_view& data)
{
	std::string spath(path);
	auto it = m_post.find(spath);
	if (it != m_post.end())
	{
		(it->second)(conn, data);
	}
}
void HttpServer::onOther(HttpConnect* conn, HttpParser* parser)
{
	if (m_other) m_other(conn, parser);
}

void HttpServer::addGet(const char * name, std::function<void(HttpConnect *, std::string_view &)> back)
{
	m_get[name] = back;
}
void HttpServer::addPost(const char * name, std::function<void(HttpConnect *, std::string_view &)> back)
{
	m_post[name] = back;
}

void HttpServer::setOther(std::function<void(HttpConnect*, HttpParser*)> back)
{
	m_other = back;
}