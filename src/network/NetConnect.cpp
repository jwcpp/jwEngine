#include "NetConnect.h"
#include "NetPacket.h"
#include "CommonPool.h"
#include "NetEvent.h"

NetConnect::NetConnect(NetEvent * nevent,uint32 buffersize) :
	_netevent(nevent),
	TcpSocket(buffersize),
	mReadPacket(NULL)
{
}

NetConnect::~NetConnect()
{
	if (mReadPacket)
	{
		recyclePacket(mReadPacket);
		mReadPacket = NULL;
	}
}

void NetConnect::on_msgbuffer(MessageBuffer * buffer)
{
	while (buffer->GetActiveSize() > 0)
	{
		if (!mReadPacket)
		{
			//create packet obj
			mReadPacket = createPacket();
			mReadPacket->wpos(0);
		}

		//read head
		if (mReadPacket->isHeadFull() == false)
		{
			uint32 rlen = mReadPacket->readHead(buffer->GetReadPointer(), buffer->GetActiveSize());
			buffer->ReadCompleted(rlen);

			if (mReadPacket->isHeadFull())
			{
				// Empty packets exit the while loop
				goto read_body;
			}
		}
		else
		{
			read_body:
			int32 needsize = mReadPacket->getMarkLen() + MSG_HEAD_SIZE - mReadPacket->wpos();
			if (needsize > 0)
			{
				int32 wsize = buffer->GetActiveSize() >= needsize ? needsize : buffer->GetActiveSize();
				if (wsize > 0)
				{
					mReadPacket->append(buffer->GetReadPointer(), wsize);
					buffer->ReadCompleted(wsize);
				}
			}

			//new packet
			if (mReadPacket->wpos() == mReadPacket->getMarkLen() + MSG_HEAD_SIZE)
			{
				_netevent->onMsg(this, mReadPacket->getMsgType(), mReadPacket);

				// recycle packet
				recyclePacket(mReadPacket);
				mReadPacket = NULL;
			}
		}
	}//while
}
void NetConnect::on_clsesocket()
{
	_netevent->onClose(this);
	_netevent->destroyConnect(this);
}


void NetConnect::sendMsg(uint32 msgtype, NetPacket * pack)
{
	NetPacket * packet = createPacket();
	packet->moveData(pack);
	packet->writeHead(msgtype);

	TcpSocket::write(packet);
}

void NetConnect::sendMsg(uint32 msgtype, const char* msg, uint32 len)
{

	NetPacket *pack = createPacket();

	pack->append(msg, len);
	pack->writeHead(msgtype);

	TcpSocket::write(pack);
}


NetPacket* NetConnect::createPacket()
{
	return new NetPacket;
}