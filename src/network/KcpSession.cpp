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

ikcpcb* KcpSessionBase::init(uint32 session_id, const sockaddr * addr, uv_udp_t * udp)
{
	__m_kcp = ikcp_create(session_id, (void *)this);
	__m_kcp->output = KcpSessionBase::__kcpOutCallback;

	memcpy((void *)&__m_addr, (const void *)addr, sizeof(sockaddr));
	__m_udp = udp;

	return __m_kcp;
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

void KcpSessionBase::flushKcp()
{
	// 每次发送完调用flush，把数据刷出去，否则要下次update才出的去
	if (__m_kcp) ikcp_flush(__m_kcp);
}

const ikcpcb* KcpSessionBase::getKcp()
{
	return __m_kcp;
}

void KcpSessionBase::setStream()
{
	// 设置成流模式后，上层逻辑需要处理消息包的边界
	if (__m_kcp) __m_kcp->stream = 1;
}

//---------------------------------------------------------------

#include "XTime.h"

KcpSession::KcpSession(KcpEvent * event_, uint32 timeout) :
	__m_kcpEvent(event_),
	__m_nTimeout(timeout),
	__m_nUpdateTime(0)
{
	__userdata = NULL;
}

KcpSession::~KcpSession()
{
}

void KcpSession::start(uint32 ms)
{
	__m_nTalkTime = XTime::iclock();
	__m_timer.start(std::bind(&KcpSession::update, this), ms, ms);
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

void KcpSession::updateKcp()
{
	uint32 currt = XTime::iclock();
	__m_nUpdateTime = KcpSessionBase::update(currt);
}

UdpPacket * KcpSession::_createPacket(int size)
{
	UdpPacket * packet = new UdpPacket;
	if (size > 0)
	{
		packet->initSize(size);
	}
	return packet;
}

void KcpSession::_recyclePacket(UdpPacket * packet)
{
	delete packet;
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
	flushKcp();
}

void KcpSession::sendMsg(uint32 msgtype, const char* msg, uint32 len)
{
	UdpPacket *pack = _createPacket();
	pack->append(msg, len);
	sendMsg(msgtype, pack);
	_recyclePacket(pack);
}