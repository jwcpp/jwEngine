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