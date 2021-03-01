#include "WebSocketPacket.h"


WebSocketPacket::WebSocketPacket()
{
	zero();
}


WebSocketPacket::~WebSocketPacket()
{
}

void WebSocketPacket::zero()
{
	this->storage().resize(WS_MAX_HEAD_SIZE);
	_wpos = WS_MAX_HEAD_SIZE;
	_rpos = WS_MAX_HEAD_SIZE;
	__m_headpos = 0;
}

int32 WebSocketPacket::getHeadSize()
{
	return WS_MAX_HEAD_SIZE;
}

void WebSocketPacket::moveData(BasePacket* packet)
{
	if (packet->getHeadSize() != getHeadSize()) return;

	int len = packet->getBodySize();
	BasePacket::moveData(packet);

	writeFrameHead(len);
}

uint32 WebSocketPacket::readFrameHead(const uint8 * pData, uint32 size)
{
	int readsize = 0;
	uint8 * pbuff = (uint8 *)pData;

	// 先读前两个字节
	int rsize = WS_HEAD_SIZE - _wpos;
	if (rsize > 0)
	{
		readsize = size >= rsize ? rsize : size;
		memcpy(__m_head + _wpos, pbuff, readsize);
		_wpos += readsize;

		if (_wpos == WS_HEAD_SIZE)
		{
			pbuff += readsize;
			size -= readsize;

			__m_headpos = WS_MAX_HEAD_SIZE - calcHeadSize();
			_wpos = __m_headpos + WS_HEAD_SIZE;
		}
	}

	// 读包头后面部分
	if (_wpos >= WS_HEAD_SIZE && size > 0)
	{
		rsize = WS_MAX_HEAD_SIZE - _wpos;
		if (rsize > 0)
		{
			rsize = size >= rsize ? rsize : size;

			put(wpos(), pbuff, rsize);
			_wpos += rsize;

			readsize += rsize;
		}
	}

	return readsize;
}

int32 WebSocketPacket::getMarkLen()
{
	uint8 paylen = getPayloadLen();
	if (paylen < 126)
		return paylen;

	if (paylen == 126)
	{
		uint16 len = getValue<uint16>(WS_HEAD_SIZE);
		return (uint32)len;
	}
	else
	{
		uint64 len = getValue<uint64>(WS_HEAD_SIZE);
		return (uint32)len;
	}
}

bool WebSocketPacket::isHeadFull()
{
	return wpos() >= WS_MAX_HEAD_SIZE;
}

int32 WebSocketPacket::calcHeadSize()
{
	uint32 headlen = WS_HEAD_SIZE;
	uint8 mask = getMask();
	uint8 paylen = getPayloadLen();

	if (paylen == 126)
	{
		headlen += 2;
	}
	else if (paylen == 127)
	{
		headlen += 8;
	}

	if (mask)
	{
		headlen += 4;
	}

	return headlen;
}

void WebSocketPacket::writeFrameHead(int32 bodylen, WSFrameType frame_type)
{
	uint8 bytelength = 0;
	__m_headpos = WS_MAX_HEAD_SIZE;
	if (bodylen <= 125)
	{
		bytelength = (uint8)bodylen;
	}
	else if (bodylen <= 0xffff)
	{
		bytelength = 126;
		__m_headpos = __m_headpos - 2;
		put(__m_headpos, (uint16)bodylen);
	}
	else
	{
		bytelength = 127;
		__m_headpos = __m_headpos - 8;
		put(__m_headpos, (uint64)bodylen);
	}
	--__m_headpos;
	put(__m_headpos, (uint8)bytelength);

	--__m_headpos;
	put(__m_headpos, (uint8)frame_type);
}

uint32 WebSocketPacket::getMaskKey()
{
	return getValue<uint32>(getHeadSize() - 4);
}

int32  WebSocketPacket::sendSize()
{
	return wpos() - __m_headpos;
}

char * WebSocketPacket::sendStream()
{
	return ((char *)contents()) + __m_headpos;
}

void WebSocketPacket::setPongPacket()
{
	if (wpos() >= sizeof(uint8))
	{
		setValue<uint8>(__m_headpos, (uint8)PONG_FRAME);
	}
}
