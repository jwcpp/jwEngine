#pragma once
/************************************************************************
* @file      HttpConnect.h
* @brief     HttpConnect
* @author    jiangwang
* @data      2020-5-20
* @version   0.1
************************************************************************/

#include "TcpSocket.h"
#include "HttpEvent.h"
#include "PoolObject.h"
#include <queue>

enum http_content_type
{
	hct_text_html ,	//HTML格式
	hct_text_plain,	//纯文本格式
	hct_text_xml,	//XML格式
	hct_text_json,	//json格式
	hct_image_gif,	//gif图片格式
	hct_image_jpeg,	//jpg图片格式
	hct_image_png,	//png图片格式

};

class BasePacket;
class MessageBuffer;
struct http_parser;
struct http_parser_url;
class HttpConnect : public TcpSocket
{
public:
	HttpConnect();
	~HttpConnect();

	void zero();
	void release();
	void setEvent(HttpEvent * e) { m_event = e; }
	void sendMsg(std::string_view sv);
	void autoMsg(std::string_view sv, enum http_content_type type = hct_text_html);
	static const char *getContentTypeStr(enum http_content_type type);
protected:
	virtual void on_msgbuffer(MessageBuffer * buffer);
	virtual void on_clsesocket();
	virtual void on_writecomplete();

private:
	static int on_url(http_parser*, const char *at, size_t length);

	bool parser(const char *, int);
	void complete();

	void send_top_msg();
	BasePacket * createPacket();
	void recyclePacket(BasePacket * pack);
private:
	HttpEvent * m_event = NULL;
	http_parser * m_parser;
	http_parser_url * m_url;
	BasePacket * m_readPacket;
	std::queue<BasePacket *> m_writePackets;
	const char * m_urlp;	//url指针
	const char * m_content;	//post数据指针
	int m_residue;
	bool m_close = true;

	INCLUDE_POOL_OBJECT
};

