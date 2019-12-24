#include "WebSocketConnect.h"
#include "WebSocketEvent.h"
#include "WebSocketPacket.h"
#include "WebSocketServer.h"
#include "EventLoop.h"
#include <stdio.h>

extern int makeWSFrameDataHeader(int len, std::vector<unsigned char>& header);
int makeWSFrameData(char* msg, int msgLen, std::vector<unsigned char>& outBuf)
{
	std::vector<unsigned char> header;
	makeWSFrameDataHeader(msgLen, header);
	outBuf.insert(outBuf.begin(), header.begin(), header.end());
	outBuf.insert(outBuf.end(), msg, msg + msgLen);
	return 0;
}

int makeWSFrameDataHeader(int len, std::vector<unsigned char>& header)
{
	header.push_back((unsigned char)BINARY_FRAME);
	if (len <= 125)
	{
		header.push_back((unsigned char)len);
	}
	else if (len <= 65535)
	{
		header.push_back((char)126);//16 bit length follows
		header.push_back((char)((len >> 8) & 0xFF));// leftmost first
		header.push_back((char)(len & 0xFF));
	}
	else // >2^16-1 (65535)
	{
		header.push_back((char)127);//64 bit length follows

		// write 8 bytes length (significant first)
		// since msg_length is int it can be no longer than 4 bytes = 2^32-1
		// padd zeroes for the first 4 bytes
		for (int i = 3; i >= 0; i--)
		{
			header.push_back((char)0);
		}
		// write the actual 32bit msg_length in the next 4 bytes
		for (int i = 3; i >= 0; i--)
		{
			header.push_back((char)((len >> 8 * i) & 0xFF));
		}
	}

	return 0;
}
//UTF-8到GB2312的转换
char* U2G(const char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

//GB2312到UTF-8的转换
char* G2U(const char* gb2312)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

class IWebEvent : public WebSocketEvent
{
public:
	virtual void onHandshake(WebSocketConnect * conn){
		printf("握手成功\n");

	};
	virtual void onAccept(WebSocketConnect * conn){
	
	};
	virtual void onConnect(WebSocketConnect * conn){
	
	};
	virtual void onClose(WebSocketConnect * conn){
	
	};
	virtual void onMsg(WebSocketConnect * conn, WebSocketPacket * pack){
		int size = pack->getHeadSize();
		const char * p = (char *)pack->contents() + size;
		p = U2G(p);
		printf("%d, %s\n", pack->getOpcode(), p);


		//conn->sendMsg("123456789\0", 10);

		//std::vector<unsigned char> msg;
		//makeWSFrameData("123456789\0", 10, msg);
		//conn->write((char *)&msg[0], msg.size());
		//conn->write((char *)pack->contents(), pack->wpos());


		std::vector<uint8> vec;
		vec.push_back(TEXT_FRAME);
		vec.push_back(4);
		vec.push_back('6');
		vec.push_back('6');
		vec.push_back('6');
		vec.push_back('6');
		conn->write((char *)&vec[0], vec.size());

	};
};


int main()
{
	EventLoop::Instance()->init();
	IWebEvent wevent;
	WebSocketServer server(EventLoop::Instance(), &wevent);
	server.listen("127.0.0.1", 8080);
	EventLoop::Instance()->Run();

	system("pause");
	return 0;
}