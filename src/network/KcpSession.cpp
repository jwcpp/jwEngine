#include "KcpSession.h"
#include "UdpPacket.h"
#include "CommonPool.h"
#include "KcpEvent.h"

KcpSessionBase::KcpSessionBase():
	__m_kcp(NULL)
{

}


KcpSessionBase::~KcpSessionBase()
{
	
}

void KcpSessionBase::init(uint32 session_id, const sockaddr * addr, uv_udp_t * udp)
{
	__m_kcp = ikcp_create(session_id, (void *)this);
	__m_kcp->output = KcpSessionBase::__kcpOutCallback;
	ikcp_nodelay(__m_kcp, 1, 10, 2, 1);

	memcpy((void *)&__m_addr, (const void *)addr, sizeof(sockaddr));
	__m_udp = udp;
}

void KcpSessionBase::destory()
{
	if (__m_kcp != NULL)
	{
		ikcp_release(__m_kcp);
		__m_kcp = NULL;
	}
}

uint32 KcpSessionBase::getSessionId()
{
	if (__m_kcp)
	{
		return __m_kcp->conv;
	}

	return 0;
}

uint32 KcpSessionBase::update(uint32 millisecond)
{
	if (!__m_kcp)
		return 0;

	ikcp_update(__m_kcp, millisecond);

	int peeksize = ikcp_peeksize(__m_kcp);
	if (peeksize > 0)
	{
		UdpPacket * packet = _createPacket(peeksize);
		if (ikcp_recv(__m_kcp, (char *)packet->contents(), peeksize) > 0)
		{
			packet->wpos(peeksize);
			_onMsg(packet);
		}
		_recyclePacket(packet);
	}

	return ikcp_check(__m_kcp, millisecond);
}

int KcpSessionBase::send(const char * buf, int len)
{
	return ikcp_send(__m_kcp, buf, len);
}

int KcpSessionBase::recv(const char * buf, long len)
{
	return ikcp_input(__m_kcp, buf, len);
}

int KcpSessionBase::__kcpOutCallback(const char * buf, int len, struct IKCPCB *kcp, void *user)
{
	KcpSessionBase * session = static_cast<KcpSessionBase *>(user);
	return session->__udpSend(buf, len);
}

int KcpSessionBase::__udpSend(const char * buf, int len)
{
	uv_buf_t msg = uv_buf_init((char *)buf, len);
	return uv_udp_try_send(__m_udp, &msg, 1, &__m_addr);
}


//---------------------------------------------------------------

#include "XTime.h"

KcpSession::KcpSession(KcpEvent * event_, uint32 timeout) :
	__m_kcpEvent(event_),
	__m_nTimeout(timeout),
	__m_nUpdateTime(0)
{
	
}

KcpSession::~KcpSession()
{
}

void KcpSession::start()
{
	__m_nTalkTime = XTime::iclock();
	__m_timer.start(std::bind(&KcpSession::update, this), 10, 10);
}

void KcpSession::over()
{
	__m_timer.stop();
}

void KcpSession::update()
{
	uint32 currt = XTime::iclock();
	if (currt >= __m_nUpdateTime)
	{
		__m_nUpdateTime = KcpSessionBase::update(currt);
	}

	if (currt > __m_nTalkTime + __m_nTimeout)
	{
		if (__m_kcpEvent)
			__m_kcpEvent->onUdpTimeout(this);
	}
}

UdpPacket * KcpSession::_createPacket(int size)
{
	UdpPacket * packet = CommPool::create<UdpPacket>();
	if (size > 0)
	{
		packet->initSize(size);
	}
	return packet;
}

void KcpSession::_recyclePacket(UdpPacket * packet)
{
	CommPool::reclaim(packet);
}

void KcpSession::_onMsg(UdpPacket * packet)
{
	__m_nTalkTime = XTime::iclock();
	if (__m_kcpEvent)
		__m_kcpEvent->onMsg(this, packet->getMsgType(), packet);
}

void KcpSession::sendMsg(uint32 msgtype, UdpPacket * pack)
{
	pack->writeHead(msgtype);
	send(pack->sendStream(), pack->sendSize());
}

void KcpSession::sendMsg(uint32 msgtype, void * msg, uint32 len)
{
	UdpPacket *pack = _createPacket();
	pack->append((uint8 *)msg, len);
	sendMsg(msgtype, pack);
	_recyclePacket(pack);
}