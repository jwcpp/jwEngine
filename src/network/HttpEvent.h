#pragma once
/************************************************************************
* @file      HttpEvent.h
* @brief     HttpEvent
* @author    jiangwang
* @data      2020-5-20
* @version   0.1
************************************************************************/

class HttpConnect;
class HttpParser;
class HttpEvent
{
public:
	virtual void onClose(HttpConnect *conn) = 0;

	virtual void onGet(HttpConnect* conn, std::string_view& path, std::string_view& data) = 0;
	virtual void onPost(HttpConnect* conn, std::string_view& path, std::string_view& data) = 0;
	virtual void onOther(HttpConnect* conn, HttpParser * parser) = 0;
};
