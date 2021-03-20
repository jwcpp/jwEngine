#include "sol/sol.hpp"
#include "TcpSocket.h"
#include "BasePacket.h"
#include "ByteBuffer.h"
#include "CommonPool.h"

void init_buffer_pool(int count)
{
	CommPool::init<CBuffer>(count);
}

void luabind_basepacket(sol::state & lua)
{
	lua["init_buffer_pool"] = &init_buffer_pool;

	lua.new_usertype<BasePacket>("BasePacket",
		"getInt8", &BasePacket::getInt8,
		"getUint8", &BasePacket::getUint8,
		"getInt16", &BasePacket::getInt16,
		"getUint16", &BasePacket::getUint16,
		"getInt32", &BasePacket::getInt32,
		"getUint32", &BasePacket::getUint32,
		"getInt64", &BasePacket::getInt64,
		"getUint64", &BasePacket::getUint64,
		"getFloat", &BasePacket::getFloat,
		"getDouble", &BasePacket::getDouble,
		"getString", &BasePacket::getString,

		"pushInt8", &BasePacket::pushInt8,
		"pushUint8", &BasePacket::pushUint8,
		"pushInt16", &BasePacket::pushInt16,
		"pushUint16", &BasePacket::pushUint16,
		"pushInt32", &BasePacket::pushInt32,
		"pushUint32", &BasePacket::pushUint32,
		"pushInt64", &BasePacket::pushInt64,
		"pushUint64", &BasePacket::pushUint64,
		"pushFloat", &BasePacket::pushFloat,
		"pushDouble", &BasePacket::pushDouble,
		"pushString", &BasePacket::pushString,
		
		"moveData", & BasePacket::moveData,
		"wpos", &BasePacket::writePos,
		"rpos", &BasePacket::readPos,
		"readData", &BasePacket::readData,
		"writeData", &BasePacket::writeData);


	lua.new_usertype<TcpSocket>("TcpSocket",
		"close", &TcpSocket::close,
		"localIP", &TcpSocket::localIP,
		"localPort", &TcpSocket::localPort,
		"remoteIP", &TcpSocket::remoteIP,
		"remotePort", &TcpSocket::remotePort,
		"setUserdata", &TcpSocket::setUserdata,
		"getUserdata", &TcpSocket::getUserdata,
		"setUsernum", &TcpSocket::setUsernum,
		"getUsernum", &TcpSocket::getUsernum);
}