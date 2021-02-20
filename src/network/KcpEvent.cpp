#include "KcpEvent.h"
#include <stdlib.h>
#include "KcpSession.h"

KcpEvent::KcpEvent()
{
}


KcpEvent::~KcpEvent()
{
}

KcpSession * KcpEvent::createConnect()
{
	return new KcpSession(this, 30 * 1000);
}

void KcpEvent::destroyConnect(KcpSession * conn)
{
	if (conn)
		delete conn;
}

int KcpEvent::onKcp(void* kcp)
{
	ikcpcb* _kcp = static_cast<ikcpcb*>(kcp);
	// int ikcp_wndsize(ikcpcb *kcp, int sndwnd, int rcvwnd);
	ikcp_nodelay(_kcp, 1, 10, 2, 1);
	// return update millisecond
	return 10; 
}