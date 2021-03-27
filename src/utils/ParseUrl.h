#pragma once

#include <string_view>

struct http_parser_url;
class ParseUrl
{
public:
	ParseUrl();
	~ParseUrl();

	int parse(const char * buf, int len);
	bool haveParam();
	std::string_view getPath();
	std::string_view getParam();

private:
	struct http_parser_url * m_url;
	const char * m_buff;
};