/************************************************************************
* @file      KcpSession.h
* @brief     kcp会话
* @author    jiangwang
* @data      2019-12-12
* @version   0.1
************************************************************************/

#pragma once

#include "BaseType.h"
#include "libuv/include/uv.h"
#include "ikcp.h"


/*
	前4字节：会话id，后1个字节：会话代码（SETUP_SESSION_CODE）
	|----|-|
*/
#define SESSION_PACKET_SIZE (4 + 1)
#define SETUP_SESSION_CODE 110

class KcpEvent;
class UdpPacket;
class KcpSessionBase
{
public:
	KcpSessionBase();
	~KcpSessionBase();

	void init(uint32 session_id, const sockaddr * addr, uv_udp_t * udp);
	void destory();
	int send(const char * buf, int len);
	int recv(const char * buf, long len);

	uint32 update(uint32 millisecond);
	uint32 getSessionId();

protected:
	virtual UdpPacket * _createPacket(int size) = 0;
	virtual void _recyclePacket(UdpPacket * packet) = 0;
	virtual void _onMsg(UdpPacket * packet) = 0;

private:
	static int __kcpOutCallback(const char * buf, int len, struct IKCPCB *kcp, void *user);
	int __udpSend(const char * buf, int len);
private:
	uv_udp_t * __m_udp;
	sockaddr   __m_addr;
	ikcpcb   * __m_kcp;
};


#include "Timer.h"
class KcpEvent;
class KcpSession : public KcpSessionBase
{
public:
	KcpSession(KcpEvent * event_, uint32 timeout);
	~KcpSession();

	void start();
	void over();
	void update();
	void sendMsg(uint32 msgtype, UdpPacket * pack);
	void sendMsg(uint32 msgtype, void * msg, uint32 len);
protected:
	virtual UdpPacket * _createPacket(int size = 0);
	virtual void _recyclePacket(UdpPacket * packet);
	virtual void _onMsg(UdpPacket * packet);

private:
	KcpEvent * __m_kcpEvent;
	Timer      __m_timer;
	uint32     __m_nTimeout;
	uint32     __m_nTalkTime;
	uint32     __m_nUpdateTime;
};
