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
	this->_storage.resize(WS_MAX_HEAD_SIZE);
	_wpos = WS_MAX_HEAD_SIZE;
	_rpos = WS_MAX_HEAD_SIZE;
	__m_headpos = WS_MAX_HEAD_SIZE - WS_HEAD_SIZE;
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

int32 WebSocketPacket::getHeadSize()
{
	return WS_MAX_HEAD_SIZE;
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

	// 写入frame类型
	put(__m_headpos, (uint8)frame_type);

	if (bodylen <= 125)
	{
		put(__m_headpos + 1, (uint8)bodylen);
	}
	else if (bodylen <= 65535)
	{
		uint8 bytelength = 126;
		put(__m_headpos + 1, (uint8)bytelength);
		put(__m_headpos + 2, (uint16)bodylen);
	}
	else
	{
		uint8 bytelength = 127;
		put(__m_headpos + 1, (uint8)bytelength);
		put(__m_headpos + 2, (uint64)bodylen);
	}

}

uint32 WebSocketPacket::getMaskKey()
{
	return getValue<uint32>(getHeadSize() - 4);
}

char * WebSocketPacket::getBodyData()
{
	return (char *)contents() + WS_MAX_HEAD_SIZE;
}

int32  WebSocketPacket::getBodySize()
{
	return wpos() - WS_MAX_HEAD_SIZE;
}

int32  WebSocketPacket::sendSize()
{
	return wpos() - __m_headpos;
}

char * WebSocketPacket::sendStream()
{
	return ((char *)contents()) + __m_headpos;
}

void WebSocketPacket::moveData(WebSocketPacket * packet)
{
	int len = packet->getBodySize();
	BasePacket::moveData(packet);
	
	//memcpy(__m_head, packet->__m_head, WS_HEAD_SIZE);
	//__m_headpos = packet->__m_headpos;

	writeFrameHead(len);
}
void WebSocketPacket::setPongPacket()
{
	if (wpos() >= sizeof(uint8))
	{
		setValue<uint8>(__m_headpos, (uint8)PONG_FRAME);
	}
}
