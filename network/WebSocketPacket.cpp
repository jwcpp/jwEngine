#include "WebSocketPacket.h"


WebSocketPacket::WebSocketPacket()
{
	fillPacketHead();
}


WebSocketPacket::~WebSocketPacket()
{
}


void WebSocketPacket::zero()
{
	_rpos = 0;
	fillPacketHead();
}

uint32 WebSocketPacket::readFrameHead(const uint8 * pData, uint32 size)
{
	int readsize = 0;
	uint8 * pbuff = (uint8 *)pData;

	// 先读前两个字节
	int rsize = WS_HEAD_SIZE - _rpos;
	if (rsize > 0)
	{
		readsize = size >= rsize ? rsize : size;
		memcpy(&_storage[_rpos], pbuff, readsize);
		_rpos += readsize;
		if (_rpos == WS_HEAD_SIZE)
		{
			_rpos = 0; //begin pos read
			*this >> __m_head[0];
			*this >> __m_head[1];

			pbuff += readsize;
			size -= readsize;
		}
	}

	// 读包头后面部分
	if (_rpos >= WS_HEAD_SIZE && size > 0)
	{
		rsize = getHeadSize() - _rpos;
		if (rsize > 0)
		{
			rsize = size >= rsize ? rsize : size;
			memcpy(&_storage[_rpos], pbuff, rsize);
			_rpos += rsize;
			readsize += rsize;
		}
	}

	return readsize;
}

uint32 WebSocketPacket::getLength()
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
	if (_rpos < WS_HEAD_SIZE)
		return false;
	if (_rpos < getHeadSize())
		return false;

	return true;
}

uint32 WebSocketPacket::getHeadSize()
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

void WebSocketPacket::writeFrameHead(WSFrameType frame_type)
{

	uint32 bodylen = _wpos - WS_RESERVER_SIZE;

	if (bodylen <= 125)
		__m_sendpos = WS_RESERVER_SIZE - 2;
	else if (bodylen <= 65535)
		__m_sendpos = WS_RESERVER_SIZE - 4;
	else
		__m_sendpos = 0;

	// 写入frame类型
	this->put(__m_sendpos, (uint8)frame_type);

	if (bodylen <= 125)
	{
		this->put(__m_sendpos + 1, (uint8)bodylen);
	}
	else if (bodylen <= 65535)
	{
		uint8 bytelength = 126;
		this->put(__m_sendpos + 1, bytelength);
		this->put(__m_sendpos + 2, (uint16)bodylen);
	}
	else
	{
		uint8 bytelength = 127;
		this->put(__m_sendpos + 1, bytelength);

		ByteConverter::apply<uint32>(&bodylen);
		this->put(__m_sendpos + 2, (uint64)bodylen);
	}
}

void WebSocketPacket::fillPacketHead()
{
	this->_storage.resize(WS_RESERVER_SIZE);
	_wpos = WS_RESERVER_SIZE;
}

uint32 WebSocketPacket::getMaskKey()
{
	return getValue<uint32>(getHeadSize() - 4);
}

const char * WebSocketPacket::getBodyData()
{
	return (const char *)contents() + getHeadSize();
}