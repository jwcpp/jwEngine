#include "WebSocketPacket.h"


WebSocketPacket::WebSocketPacket()
{
	_fillHead();
}


WebSocketPacket::~WebSocketPacket()
{
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
		append(pbuff, readsize);
		if (_wpos == WS_HEAD_SIZE)
		{
			*this >> __m_head[0];
			*this >> __m_head[1];

			pbuff += readsize;
			size -= readsize;

			__m_bodypos = calcHeadSize();
		}
	}

	// 读包头后面部分
	if (_wpos >= WS_HEAD_SIZE && size > 0)
	{
		rsize = getHeadSize() - _wpos;
		if (rsize > 0)
		{
			rsize = size >= rsize ? rsize : size;
			append(pbuff, rsize);
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
	if (_wpos < WS_HEAD_SIZE)
		return false;
	if (_wpos < getHeadSize())
		return false;

	return true;
}

int32 WebSocketPacket::getHeadSize()
{
	return __m_bodypos;
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
	*this << (uint8)frame_type;

	if (bodylen <= 125)
	{
		*this << (uint8)bodylen;
	}
	else if (bodylen <= 65535)
	{
		uint8 bytelength = 126;
		*this << (uint8)bytelength;
		*this << (uint16)bodylen;
	}
	else
	{
		uint8 bytelength = 127;
		*this << bytelength;
		ByteConverter::apply<uint32>((uint32 *)&bodylen);
		*this << (uint64)bodylen;
	}

	__m_bodypos = wpos();
}

void WebSocketPacket::_fillHead()
{
	this->_storage.resize(0);
	_wpos = 0;
	_rpos = 0;
	__m_bodypos = 0;
}

uint32 WebSocketPacket::getMaskKey()
{
	return getValue<uint32>(getHeadSize() - 4);
}

char * WebSocketPacket::getBodyData()
{
	return (char *)contents() + __m_bodypos;
}

int32  WebSocketPacket::getBodySize()
{
	return wpos() - __m_bodypos;
}

int32  WebSocketPacket::sendSize()
{
	return wpos();
}

char * WebSocketPacket::sendStream()
{
	return (char *)contents();
}

void WebSocketPacket::moveData(WebSocketPacket * packet)
{
	writeFrameHead(packet->getBodySize());
	append(packet->getBodyData(), packet->getBodySize());
}