/************************************************************************
* @file      KcpClient.h
* @brief     kcp client
* @author    jiangwang
* @data      2019-12-13
* @version   0.1
************************************************************************/

#pragma once

#include "UdpClient.h"
#include "BaseType.h"
#include <map>

class KcpEvent;
class EventLoop;
class KcpSession;
class KcpClient : public UdpClient
{
public:
	KcpClient(EventLoop * loop, KcpEvent * event_);
	~KcpClient();

	void createSession(const char * server_ip, int server_port, bool ipv6 = false);
	void closeSession(KcpSession * session);

protected:
	virtual void onMessage(const sockaddr * addr, const char * buff, ssize_t len);

private:
	void release(KcpSession * session);
private:
	KcpEvent * __m_kcpEvent;
	std::map<uint32, KcpSession *>  __m_mapSessions;
};

