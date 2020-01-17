#include "BasePacket.h"


BasePacket::BasePacket()
{
	_fillHead();
}

BasePacket::~BasePacket()
{
}


void BasePacket::zero()
{
	_rpos = 0;
	_fillHead();
}

void BasePacket::moveData(BasePacket * packet)
{
	rpos(packet->rpos());
	wpos(packet->wpos());
	_storage = packet->Move();
}

void BasePacket::_fillHead()
{

}

int32  BasePacket::getBodySize()
{
	return 0;
}

char * BasePacket::getBodyData()
{
	return NULL;
}

// read msg call
int32  BasePacket::getHeadSize()
{
	return 0;
}

// message head mark length
int32  BasePacket::getMarkLen()
{
	return 0;
}

// send msg call
int32  BasePacket::sendSize()
{
	return 0;
}

char * BasePacket::sendStream()
{
	return NULL;
}