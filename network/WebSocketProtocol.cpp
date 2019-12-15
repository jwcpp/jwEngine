#include "WebSocketProtocol.h"

/*-------------------------------------------------------------------


0                   1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-------+-+-------------+-------------------------------+
|F|R|R|R| opcode|M| Payload len |    Extended payload length    |
|I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
|N|V|V|V|       |S|             |   (if payload len==126/127)   |
| |1|2|3|       |K|             |                               |
+-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
|     Extended payload length continued, if payload len == 127  |
+ - - - - - - - - - - - - - - - +-------------------------------+
|                               |Masking-key, if MASK set to 1  |
+-------------------------------+-------------------------------+
| Masking-key (continued)       |          Payload Data         |
+-------------------------------- - - - - - - - - - - - - - - - +
:                     Payload Data continued ...                :
+ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
|                     Payload Data continued ...                |
+---------------------------------------------------------------+

1. FIN：表示这个数据是不是接收完毕，为1表示收到的数据是完整的，占1bit
2. RSV1～3：用于扩展，通常都为0，各占1bit
3. OPCODE：表示报文的类型，占4bit  
   (1). 0x00：标识一个中间数据包
   (2). 0x01：标识一个text数据包
   (3). 0x02：标识一个二进制数据包
   (4). 0x03～07：保留
   (5). 0x08：标识一个断开连接数据包
   (6). 0x09：标识一个ping数据包
   (7). 0x0A：标识一个pong数据包
   (8). 0x0B～F：保留
4. MASK：用于表示数据是否经常掩码处理，为1时，Masking-key即存在，占1bit
5. Payload len：表示数据长度，即Payload Data的长度，当Payload len为0～125时，表示的值就是Payload Data的真实长度；
   当Payload len为126时，报文其后的2个字节形成的16bits无符号整型数的值是Payload Data的真实长度（网络字节序，需转换）；
   当Payload len为127时，报文其后的8个字节形成的64bits无符号整型数的值是Payload Data的真实长度（网络字节序，需转换）；
6. Masking-key：掩码，当Mask为1时存在，占4字节32bit
7. Payload Data：表示数据

-----------------------------------------------------------------------*/


#include <sstream>
#include <map>
#include "base64.h"
#include "sha1.h"
#include "Tools.h"

//for htonl()
#include "Platform.h"
#ifdef SYSTEM_WIN
	#include<WinSock2.h>
#else
	#include <arpa/inet.h>
#endif

WebSocketProtocol::WebSocketProtocol()
{
}


WebSocketProtocol::~WebSocketProtocol()
{
}

/*
GET /chat HTTP/1.1
Host: server.example.com
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==
Origin: http://example.com
Sec-WebSocket-Protocol: chat, superchat
Sec-WebSocket-Version: 13
*/


const char * ackHandshake = "HTTP/1.1 101 Switching Protocols\r\n"
	"Upgrade: websocket\r\n"
	"Connection: Upgrade\r\n"
	"Sec-WebSocket-Accept: %s\r\n"
	"WebSocket-Location: ws://%s/WebManagerSocket\r\n"
	"WebSocket-Protocol: WebManagerSocket\r\n\r\n";

bool WebSocketProtocol::parseHandshake(const char * pData, int len)
{
	if (len < 2)
		return false;

	std::istringstream stream(pData);
	std::string line;
	std::getline(stream, line);
	if (line.find("GET", 0) != 0)
		return false;

	std::map<std::string, std::string> mapKey;
	while (std::getline(stream, line))
	{
		std::string::size_type pos = line.find(": ");
		mapKey[std::string(line.c_str(), 0, pos)] = std::string(line.c_str() + pos + 2);
	}

	auto ithost = mapKey.find("Host");
	if (ithost == mapKey.end())
		return false;

	auto itkey = mapKey.find("Sec-WebSocket-Key");
	if (itkey == mapKey.end())
		return false;

	__m_strHost = ithost->second;
	__m_strKey = itkey->second;
	return true;
}

std::string WebSocketProtocol::respondHandshake()
{
	static const char * s_key = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
	std::string server_key = __m_strKey + s_key;

	SHA1 sha1;
	unsigned int message_digest[5];

	sha1 << server_key.c_str();
	sha1.Result(message_digest);
	for (int i = 0; i < 5; ++i)
		message_digest[i] = htonl(message_digest[i]);

	server_key = base64_encode(reinterpret_cast<const unsigned char*>(message_digest), 20);
	return Tools::format(ackHandshake, server_key.c_str(), __m_strHost.c_str());
}