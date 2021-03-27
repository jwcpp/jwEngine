#include "HttpParser.h"

struct HttpSettings : public llhttp_settings_t
{
public:
	HttpSettings()
	{
		llhttp_settings_init(this);

		this->on_url = [](llhttp_t* _, const char* at, size_t length)->int {
			HttpParser* parser = (HttpParser*)(_->data);
			return parser->getUrl()->parse(at, length);
		};
	}
};

HttpSettings http_settings;

/*static*/ llhttp_settings_t* HttpParser::getSettings()
{
	return &http_settings;
}

HttpParser::HttpParser()
{

}

void HttpParser::zero()
{
	llhttp_init(&m_parser, HTTP_REQUEST, &http_settings);
	m_parser.data = this;
}
#include <stdio.h>
bool HttpParser::parser(const char* buf, int len)
{
	enum llhttp_errno err = llhttp_execute(&m_parser, buf, len);
	if (err != HPE_OK)
	{
		fprintf(stderr, "Parse error: %s %s\n", llhttp_errno_name(err), m_parser.reason);
		return false;
	}

	return true;
}
