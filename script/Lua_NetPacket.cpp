#include "NetPacket.h"
#include <string>
#include "sol/sol.hpp"


class Lua_NetPacket : public NetPacket
{
public:
	
	template<typename T>
	T getValue() {
		T t;
		*this >> t;
		return t;
	}

	//------->get
	int8 getInt8() {
		return getValue<int8>();
	}
	uint8 getUint8() {
		return getValue<uint8>();
	}
	int16 getInt16() {
		return getValue<int16>();
	}
	uint16 getUint16() {
		return getValue<uint16>();
	}
	int32 getInt32() {
		return getValue<int32>();
	}
	uint32 getUint32() {
		return getValue<uint32>();
	}
	int64 getInt64() {
		return getValue<int64>();
	}
	uint64 getUint64() {
		return getValue<uint64>();
	}
	float getFloat() {
		return getValue<float>();
	}
	double getDouble() {
		return getValue<double>();
	}

	std::string getString() {
		return getValue<std::string>();
	}


	//------->push

	void pushInt8(int8 value) {
		*this << value;
	}
	void pushUint8(uint8 value) {
		*this << value;
	}
	void pushInt16(int16 value) {
		*this << value;
	}
	void pushUint16(uint16 value) {
		*this << value;
	}
	void pushInt32(int32 value) {
		*this << value;
	}
	void pushUint32(uint32 value) {
		*this << value;
	}
	void pushInt64(int64 value) {
		*this << value;
	}
	void pushUint64(uint64 value) {
		*this << value;
	}
	void pushFloat(float value) {
		*this << value;
	}
	void pushDouble(double value) {
		*this << value;
	}
	void pushString(std::string value) {
		*this << value;
	}

};


void luabind_netpacket(sol::state & lua)
{
	lua.new_usertype<Lua_NetPacket>("NetPacket",
		"getInt8", &Lua_NetPacket::getInt8,
		"getUint8", &Lua_NetPacket::getUint8,
		"getInt16", &Lua_NetPacket::getInt16,
		"getUint16", &Lua_NetPacket::getUint16,
		"getInt32", &Lua_NetPacket::getInt32,
		"getUint32", &Lua_NetPacket::getUint32,
		"getInt64", &Lua_NetPacket::getInt64,
		"getUint64", &Lua_NetPacket::getUint64,
		"getFloat", &Lua_NetPacket::getFloat,
		"getDouble", &Lua_NetPacket::getDouble,
		"getString", &Lua_NetPacket::getString,
		
		"pushInt8", &Lua_NetPacket::pushInt8,
		"pushUint8", &Lua_NetPacket::pushUint8,
		"pushInt16", &Lua_NetPacket::pushInt16,
		"pushUint16", &Lua_NetPacket::pushUint16,
		"pushInt32", &Lua_NetPacket::pushInt32,
		"pushUint32", &Lua_NetPacket::pushUint32,
		"pushInt64", &Lua_NetPacket::pushInt64,
		"pushUint64", &Lua_NetPacket::pushUint64,
		"pushFloat", &Lua_NetPacket::pushFloat,
		"pushDouble", &Lua_NetPacket::pushDouble,
		"pushString", &Lua_NetPacket::pushString);
}