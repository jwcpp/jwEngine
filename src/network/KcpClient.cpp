#include "KcpClient.h"
#include "KcpEvent.h"
#include "KcpSession.h"


KcpClient::KcpClient(EventLoop * loop, KcpEvent * event_):
	UdpClient(loop),
	__m_kcpEvent(event_)
{
}


KcpClient::~KcpClient()
{
}


void KcpClient::createSession(const char * server_ip, int server_port, bool ipv6)
{
	connect(server_ip, server_port, ipv6);
	
	uint8 msg[SESSION_PACKET_SIZE];
	*(uint32 *)msg = 0;
	*(uint8 *)(msg + 4) = SETUP_SESSION_CODE;
	writeToServer((char *)msg, SESSION_PACKET_SIZE);
}

void KcpClient::closeSession(KcpSession * session)
{
	std::map<uint32, KcpSession *>::iterator it = __m_mapSessions.find(session->getSessionId());
	if (it != __m_mapSessions.end())
	{
		__m_mapSessions.erase(session->getSessionId());
		release(session);
	}
}

void KcpClient::onMessage(const sockaddr * addr, const char * buff, ssize_t len)
{
	uint32 conv = ikcp_getconv((const void *)buff);
	unsigned char code = *(unsigned char*)(buff + 4);
	if (len == SESSION_PACKET_SIZE && conv > 0 && code == SETUP_SESSION_CODE)
	{
		KcpSession * session = __m_kcpEvent->createConnect();
		uint32 ms = __m_kcpEvent->onKcp(session->init(conv, addr, &_udp));
		session->start(ms);
		__m_mapSessions[conv] = session;
		__m_kcpEvent->onConnect(session);

	}
	else
	{
		std::map<uint32, KcpSession *>::iterator it = __m_mapSessions.find(conv);
		if (it != __m_mapSessions.end())
		{
			KcpSession * session = it->second;
			if (session->recv(buff, len) == 0) {
				session->updateKcp();
			}else{
				release(session);
				__m_mapSessions.erase(it);
			}
		}
	}
	
}

void KcpClient::release(KcpSession * session)
{
	session->destory();
	session->over();
	__m_kcpEvent->onClose(session);
	__m_kcpEvent->destroyConnect(session);
}