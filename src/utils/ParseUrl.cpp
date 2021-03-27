#include "ParseUrl.h"
#include "http_parser.h"


ParseUrl::ParseUrl()
{
	m_buff = NULL;
	m_url = (struct http_parser_url*)malloc(sizeof(struct http_parser_url));
	http_parser_url_init(m_url);
}

ParseUrl::~ParseUrl()
{
	if (m_url) {
		free(m_url);
		m_url = NULL;
	}
}

int ParseUrl::parse(const char* buf, int len)
{
	m_buff = buf;
	return http_parser_parse_url(buf, len, 0, m_url);
}

bool ParseUrl::haveParam()
{
	return ((m_url->field_set & (1 << UF_QUERY)) != 0);
}

std::string_view ParseUrl::getPath()
{
	return std::string_view(m_buff + m_url->field_data[UF_PATH].off, m_url->field_data[UF_PATH].len);
}
std::string_view ParseUrl::getParam()
{
	return std::string_view(m_buff + m_url->field_data[UF_QUERY].off, m_url->field_data[UF_QUERY].len);
}