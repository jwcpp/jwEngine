#pragma once

#include "llhttp.h"
#include "ParseUrl.h"

// 
struct HttpSettings : public llhttp_settings_t
{
public:
	HttpSettings();
};

HttpSettings* getHttpSettings();

class HttpParser : public llhttp_t
{
public:
	HttpParser();
	void zero();

	bool parser(const char* buf, int len);
	uint64_t contentLen() { return llhttp_t::content_length; }
	uint8_t method() { return llhttp_t::method; }
	bool isClose() { return llhttp_should_keep_alive(this) == 0; }
	ParseUrl* getUrl() { return &m_url; }
	llhttp_t* getParser() { return this; }
private:
	ParseUrl m_url;
};

