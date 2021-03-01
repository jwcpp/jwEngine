
#ifndef _BYTEBUFFER_H
#define _BYTEBUFFER_H

#include "ByteConverter.h"
#include <string>
#include <vector>
#include <cstring>
#include "PoolObject.h"

class CBuffer : public PoolObject, public std::vector<uint8> {
public:
	void zero() {}
	void release() {}
};

class ByteBuffer
{
public:
	static size_t const DEFAULT_SIZE = 0x1000;

	// constructor
	ByteBuffer();
	ByteBuffer(size_t reserve);
	ByteBuffer(ByteBuffer&& buf);
	ByteBuffer(ByteBuffer const& right);
	virtual ~ByteBuffer();
	ByteBuffer& operator=(ByteBuffer const& right);
	ByteBuffer& operator=(ByteBuffer&& right);
	
	CBuffer* createBuffer();
	void swapBuffer(ByteBuffer& right);
	CBuffer& storage() { return *_storage; }

	void clear()
	{
		_rpos = 0;
		_wpos = 0;
		_storage->clear();
	}

	template <typename T>
	void append(T value)
	{
		static_assert(std::is_trivially_copyable<T>::value, "append(T) must be used with trivially copyable types");
		EndianConvert(value);
		append((uint8 *)&value, sizeof(value));
	}

	template <typename T>
	void put(std::size_t pos, T value)
	{
		static_assert(std::is_trivially_copyable<T>::value, "put(size_t, T) must be used with trivially copyable types");
		EndianConvert(value);
		put(pos, (uint8 *)&value, sizeof(value));
	}

	ByteBuffer &operator<<(uint8 value)
	{
		append<uint8>(value);
		return *this;
	}

	ByteBuffer &operator<<(uint16 value)
	{
		append<uint16>(value);
		return *this;
	}

	ByteBuffer &operator<<(uint32 value)
	{
		append<uint32>(value);
		return *this;
	}

	ByteBuffer &operator<<(uint64 value)
	{
		append<uint64>(value);
		return *this;
	}

	// signed as in 2e complement
	ByteBuffer &operator<<(int8 value)
	{
		append<int8>(value);
		return *this;
	}

	ByteBuffer &operator<<(int16 value)
	{
		append<int16>(value);
		return *this;
	}

	ByteBuffer &operator<<(int32 value)
	{
		append<int32>(value);
		return *this;
	}

	ByteBuffer &operator<<(int64 value)
	{
		append<int64>(value);
		return *this;
	}

	// floating points
	ByteBuffer &operator<<(float value)
	{
		append<float>(value);
		return *this;
	}

	ByteBuffer &operator<<(double value)
	{
		append<double>(value);
		return *this;
	}

	ByteBuffer &operator<<(const std::string &value)
	{
		if (size_t len = value.length())
			append((uint8 const*)value.c_str(), len);
		append<uint8>(0);
		return *this;
	}

	ByteBuffer &operator<<(const char *str)
	{
		if (size_t len = (str ? strlen(str) : 0))
			append((uint8 const*)str, len);
		append<uint8>(0);
		return *this;
	}

	ByteBuffer &operator>>(bool &value)
	{
		value = read<char>() > 0 ? true : false;
		return *this;
	}

	ByteBuffer &operator>>(uint8 &value)
	{
		value = read<uint8>();
		return *this;
	}

	ByteBuffer &operator>>(uint16 &value)
	{
		value = read<uint16>();
		return *this;
	}

	ByteBuffer &operator>>(uint32 &value)
	{
		value = read<uint32>();
		return *this;
	}

	ByteBuffer &operator>>(uint64 &value)
	{
		value = read<uint64>();
		return *this;
	}

	//signed as in 2e complement
	ByteBuffer &operator>>(int8 &value)
	{
		value = read<int8>();
		return *this;
	}

	ByteBuffer &operator>>(int16 &value)
	{
		value = read<int16>();
		return *this;
	}

	ByteBuffer &operator>>(int32 &value)
	{
		value = read<int32>();
		return *this;
	}

	ByteBuffer &operator>>(int64 &value)
	{
		value = read<int64>();
		return *this;
	}

	ByteBuffer &operator>>(float &value);
	ByteBuffer &operator>>(double &value);

	ByteBuffer &operator>>(std::string& value)
	{
		value.clear();
		while (rpos() < size())                         // prevent crash at wrong string format in packet
		{
			char c = read<char>();
			if (c == 0)
				break;
			value += c;
		}
		return *this;
	}

	uint8& operator[](size_t const pos)
	{
		return (*_storage)[pos];
	}

	uint8 const& operator[](size_t const pos) const
	{
		return (*_storage)[pos];
	}

	size_t rpos() const { return _rpos; }

	size_t rpos(size_t rpos_)
	{
		_rpos = rpos_;
		return _rpos;
	}

	void rfinish()
	{
		_rpos = wpos();
	}

	size_t wpos() const { return _wpos; }

	size_t wpos(size_t wpos_)
	{
		_wpos = wpos_;
		return _wpos;
	}


	template<typename T>
	void read_skip() { read_skip(sizeof(T)); }

	void read_skip(size_t skip)
	{
		_rpos += skip;
	}

	template <typename T>
	T read()
	{
		T r = read<T>(_rpos);
		_rpos += sizeof(T);
		return r;
	}

	template <typename T>
	T read(size_t pos) const
	{
		T val = *((T const*)&(*_storage)[pos]);
		EndianConvert(val);
		return val;
	}

	template<class T>
	void read(T* dest, size_t count)
	{
		static_assert(std::is_trivially_copyable<T>::value, "read(T*, size_t) must be used with trivially copyable types");
		return read(reinterpret_cast<uint8*>(dest), count * sizeof(T));
	}

	void read(uint8 *dest, size_t len)
	{
		std::memcpy(dest, &(*_storage)[_rpos], len);
		_rpos += len;
	}

	std::string ReadString(uint32 length)
	{
		if (!length)
			return std::string();

		std::string str((char const*)&(*_storage)[_rpos], length);
		_rpos += length;
		return str;
	}

	//! Method for writing strings that have their length sent separately in packet
	//! without null-terminating the string
	void WriteString(std::string const& str)
	{
		if (size_t len = str.length())
			append(str.c_str(), len);
	}

	void WriteString(char const* str, size_t len)
	{
		if (len)
			append(str, len);
	}

	uint8* contents()
	{
		return _storage->data();
	}

	uint8 const* contents() const
	{
		return _storage->data();
	}

	size_t size() const { return _storage->size(); }
	bool empty() const { return _storage->empty(); }

	void resize(size_t newsize)
	{
		_storage->resize(newsize, 0);
		_rpos = 0;
		_wpos = size();
	}

	void reserve(size_t ressize)
	{
		if (ressize > size())
			_storage->reserve(ressize);
	}

	void append(const char *src, size_t cnt)
	{
		return append((const uint8 *)src, cnt);
	}

	template<class T>
	void append(const T *src, size_t cnt)
	{
		return append((const uint8 *)src, cnt * sizeof(T));
	}

	void append(const uint8 *src, size_t cnt);

	void append(const ByteBuffer& buffer)
	{
		if (!buffer.empty())
			append(buffer.contents(), buffer.size());
	}

	void put(size_t pos, const uint8 *src, size_t cnt);

protected:
	size_t _rpos, _wpos;
	//std::vector<uint8> _storage;
	CBuffer* _storage;
};

/// @todo Make a ByteBuffer.cpp and move all this inlining to it.
template<> inline std::string ByteBuffer::read<std::string>()
{
	std::string tmp;
	*this >> tmp;
	return tmp;
}

template<>
inline void ByteBuffer::read_skip<char*>()
{
	std::string temp;
	*this >> temp;
}

template<>
inline void ByteBuffer::read_skip<char const*>()
{
	read_skip<char*>();
}

template<>
inline void ByteBuffer::read_skip<std::string>()
{
	read_skip<char*>();
}

#endif