#include "KcpServer.h"
#include "ikcp.h"
#include "KcpSession.h"
#include "KcpEvent.h"

uint32 gSessionid = 0;

KcpServer::KcpServer(EventLoop * loop, KcpEvent * event_):
	UdpServer(loop),
	__m_kcpEvent(event_)
{
}


KcpServer::~KcpServer()
{
}

void KcpServer::closeSession(KcpSession * session)
{
	std::map<uint32, KcpSession *>::iterator it = __m_mapSessions.find(session->getSessionId());
	if (it != __m_mapSessions.end())
	{
		__m_mapSessions.erase(session->getSessionId());
		release(session);
	}
}

void KcpServer::shutdown()
{
	uv_close((uv_handle_t *)&_m_udp, NULL);
}

void KcpServer::onMessage(const sockaddr * addr, const char * buff, ssize_t len)
{
	if (len <= 0)
	{
		return;
	}

	uint32 conv = ikcp_getconv((const void *)buff);
	unsigned char code = *(unsigned char*)(buff + 4);
	if (len == SESSION_PACKET_SIZE && conv == 0 && code == SETUP_SESSION_CODE)
	{
		gSessionid++;


		//create new session
		KcpSession * session = __m_kcpEvent->createConnect();
		uint32 ms = __m_kcpEvent->onKcp(session->init(gSessionid, addr, &_m_udp));
		session->start(ms);
		__m_mapSessions[gSessionid] = session;

		//send to client
		uint8 msg[SESSION_PACKET_SIZE];
		*(uint32 *)msg = gSessionid;
		*(uint8 *)(msg + 4) = SETUP_SESSION_CODE;
		uv_buf_t uvbuff = uv_buf_init((char *)msg, len);
		uv_udp_try_send(&_m_udp, &uvbuff, 1, addr);

		__m_kcpEvent->onAccept(session);
	}
	else
	{
		std::map<uint32, KcpSession *>::iterator it = __m_mapSessions.find(conv);
		if (it != __m_mapSessions.end())
		{
			KcpSession * session = it->second;
			if (session->recv(buff, len) == 0){
				session->updateKcp();
			}else{
				release(session);
				__m_mapSessions.erase(it);
			}
		}
	}
}

void KcpServer::release(KcpSession * session)
{
	session->destory();
	session->over();
	__m_kcpEvent->onClose(session);
	__m_kcpEvent->destroyConnect(session);
}