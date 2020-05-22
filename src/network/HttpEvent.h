#pragma once
/************************************************************************
* @file      HttpEvent.h
* @brief     HttpEvent
* @author    jiangwang
* @data      2020-5-20
* @version   0.1
************************************************************************/

class HttpConnect;
class HttpEvent
{
public:
	virtual void onClose(HttpConnect *conn) = 0;
	virtual void onMsg(HttpConnect *conn, int method, std::string_view & path, std::string_view & data) = 0;
};
