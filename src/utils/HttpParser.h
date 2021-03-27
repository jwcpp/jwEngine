#pragma once

#include "llhttp.h"
#include "ParseUrl.h"

class HttpParser
{
public:
	HttpParser();
	void zero();

	static llhttp_settings_t* getSettings();

	bool parser(const char* buf, int len);
	uint64_t contentLen() { return m_parser.content_length; }
	uint8_t method() { return m_parser.method; }
	bool isClose() { return llhttp_should_keep_alive(&m_parser) == 0; }
	ParseUrl* getUrl() { return &m_url; }
	llhttp_t* getParser() { return &m_parser; }
private:
	llhttp_t m_parser;
	ParseUrl m_url;
};

