#include "HttpParser.h"

HttpSettings::HttpSettings()
{
	llhttp_settings_init(this);

	this->on_url = [](llhttp_t* _, const char* at, size_t length)->int {
		HttpParser* parser = (HttpParser*)(_);
		return parser->getUrl()->parse(at, length);
	};
}

HttpSettings http_settings;

HttpSettings* getHttpSettings()
{
	return &http_settings;
}

HttpParser::HttpParser()
{

}

void HttpParser::zero()
{
	llhttp_init(this, HTTP_REQUEST, &http_settings);
}

#include <stdio.h>
bool HttpParser::parser(const char* buf, int len)
{
	enum llhttp_errno err = llhttp_execute(this, buf, len);
	if (err != HPE_OK)
	{
		fprintf(stderr, "Parse error: %s %s\n", llhttp_errno_name(err), llhttp_t::reason);
		return false;
	}

	return true;
}
