/************************************************************************
* @file      KcpServer.h
* @brief     kcp server
* @author    jiangwang
* @data      2019-12-12
* @version   0.1
************************************************************************/

#pragma once

#include "UdpServer.h"
#include <map>
#include "BaseType.h"

class KcpEvent;
class EventLoop;
class KcpSession;
class KcpServer : public UdpServer
{
public:
	KcpServer(EventLoop * loop, KcpEvent * event_);
	~KcpServer();

	void closeSession(KcpSession * session);
	void shutdown();

protected:
	virtual void onMessage(const sockaddr * addr, const char * buff, ssize_t len);
	
private:
	void release(KcpSession * session);
private:
	KcpEvent * __m_kcpEvent;
	std::map<uint32, KcpSession *> __m_mapSessions;
};

