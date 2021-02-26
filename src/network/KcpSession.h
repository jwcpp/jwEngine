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
	virtual ~KcpSessionBase();

	ikcpcb* init(uint32 session_id, const sockaddr * addr, uv_udp_t * udp);
	void destory();
	int send(const char * buf, int len);
	int recv(const char * buf, long len);

	uint32 update(uint32 millisecond);
	uint32 getSessionId();
	void flushKcp();
	const ikcpcb* getKcp();
	void setStream();
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


#include "UTimer.h"
class KcpEvent;
class KcpSession : public KcpSessionBase
{
public:
	KcpSession(KcpEvent * event_, uint32 timeout);
	~KcpSession();

	void start(uint32 ms);
	void over();
	void update();
	void updateKcp();
	void sendMsg(uint32 msgtype, UdpPacket * pack);
	void sendMsg(uint32 msgtype, const char * msg, uint32 len);
	// lua call
	void sendPacket(uint32 msgtype, UdpPacket * pack) { sendMsg(msgtype, pack); }
	void sendData(uint32 msgtype, std::string_view sv) { sendMsg(msgtype, sv.data(), sv.size()); }

	void setUserdata(void * userdata) { __userdata = userdata; }
	void * getUserdata() { return __userdata; }
protected:
	virtual UdpPacket * _createPacket(int size = 0);
	virtual void _recyclePacket(UdpPacket * packet);
	virtual void _onMsg(UdpPacket * packet);

private:
	KcpEvent * __m_kcpEvent;
	UTimer      __m_timer;
	uint32     __m_nTimeout;
	uint32     __m_nTalkTime;
	uint32     __m_nUpdateTime;
	void   *   __userdata;
};
