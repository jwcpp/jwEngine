#include <cstring>
#include "ByteBuffer.h"


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
	_storage.insert(_storage.begin() + _wpos, src, src + cnt);
	_wpos += cnt;
}

void ByteBuffer::put(size_t pos, const uint8 *src, size_t cnt)
{
	std::memcpy(&_storage[pos], src, cnt);
}
