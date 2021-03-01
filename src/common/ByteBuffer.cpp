#include <cstring>
#include "ByteBuffer.h"
#include "CommonPool.h"

// constructor
ByteBuffer::ByteBuffer() : _rpos(0), _wpos(0)
{
	_storage = createBuffer();
	_storage->reserve(DEFAULT_SIZE);
}

ByteBuffer::ByteBuffer(size_t reserve) : _rpos(0), _wpos(0)
{
	_storage = createBuffer();
	_storage->reserve(reserve);
}

ByteBuffer::ByteBuffer(ByteBuffer&& buf) : _rpos(buf._rpos), _wpos(buf._wpos)
{ 
	_storage = createBuffer();
	swapBuffer(buf);
}

ByteBuffer::ByteBuffer(ByteBuffer const& right) : _rpos(right._rpos), _wpos(right._wpos)
{
	_storage = createBuffer();
	*_storage = *right._storage;
}

ByteBuffer& ByteBuffer::operator=(ByteBuffer const& right)
{
	if (this != &right)
	{
		_rpos = right._rpos;
		_wpos = right._wpos;
		*_storage = *right._storage;
	}

	return *this;
}

ByteBuffer& ByteBuffer::operator=(ByteBuffer&& right)
{
	if (this != &right)
	{
		_rpos = right._rpos;
		_wpos = right._wpos;
		swapBuffer(right);
	}

	return *this;
}

ByteBuffer::~ByteBuffer() {
	if(_storage) CommPool::reclaim(_storage);
}

CBuffer* ByteBuffer::createBuffer()
{
	return CommPool::create<CBuffer>();
}
void ByteBuffer::swapBuffer(ByteBuffer& right)
{
	CBuffer* tmp = _storage;
	_storage = right._storage;
	right._storage = tmp;
}

ByteBuffer& ByteBuffer::operator>>(float& value)
{
	value = read<float>();
	return *this;
}

ByteBuffer& ByteBuffer::operator>>(double& value)
{
	value = read<double>();
	return *this;
}

void ByteBuffer::append(const uint8 *src, size_t cnt)
{
	_storage->insert(_storage->begin() + _wpos, src, src + cnt);
	_wpos += cnt;
}

void ByteBuffer::put(size_t pos, const uint8 *src, size_t cnt)
{
	std::memcpy(&(*_storage)[pos], src, cnt);
}
