#include <stdio.h>
#include <string>
#include <iostream>
#include "http_parser.h"
#include <string_view>


int main()
{
	http_parser_settings settings;
	settings.on_message_begin = [](http_parser*) { printf("begin\n"); return 0; };
	settings.on_url = [](http_parser*, const char *at, size_t length) { std::string s(at, length); std::cout<<"on_url:"<<s<<std::endl; return 0; };
	settings.on_status = [](http_parser*, const char *at, size_t length) { std::string s(at, length); std::cout << "on_status:" << s << std::endl; return 0; };
	settings.on_header_field = [](http_parser*, const char *at, size_t length) { std::string s(at, length); std::cout << "on_header_field:" << s << std::endl; return 0; };
	settings.on_header_value = [](http_parser*, const char *at, size_t length) { std::string s(at, length); std::cout << "on_header_value:" << s << std::endl; return 0; };
	settings.on_headers_complete = [](http_parser*) { printf("on_headers_complete\n"); return 0; };
	settings.on_body = [](http_parser*, const char *at, size_t length) { std::string s(at, length); std::cout << "on_body:" << s << std::endl; return 0; };
	settings.on_message_complete = [](http_parser*) { printf("on_message_complete\n"); return 0; };
	/* When on_chunk_header is called, the current chunk length is stored
	 * in parser->content_length.
	 */
	settings.on_chunk_header = [](http_parser*) { printf("on_chunk_header\n"); return 0; };
	settings.on_chunk_complete = [](http_parser*) { printf("on_chunk_complete\n"); return 0; };

	//const char * buf = "POST /login HTTP/1.1\r\nHost: admin.omsg.cn\r\nAccept: */*\r\nConnection: Keep-Alive\r\nContent-Length: 3\r\n\r\n";
	const char * buf = "GET /login?a=111 HTTP/1.1\r\nHost: admin.omsg.cn\r\nAccept: */*\r\nConnection: Keep-Alive\r\n";

	http_parser *parser = (http_parser *)malloc(sizeof(http_parser));
	http_parser_init(parser, HTTP_REQUEST);
	int parsed = http_parser_execute(parser, &settings, buf, strlen(buf));

	std::cout << std::endl;
	//std::cout << parsed <<": "<< parser->method << std::endl;
	if (parser->upgrade) {
		/* handle new protocol */
	}
	else if (parser->http_errno != HPE_OK) {
		/* Handle error. Usually just close the connection. */
		std::cout <<"error: "<< http_errno_description((enum http_errno)parser->http_errno) << std::endl;
	}

	std::cout << "content_length£º" << parser->content_length << std::endl;


	char *url = "/users?a=1";
	struct http_parser_url u;
	int err = http_parser_parse_url(url, strlen(url), 0, &u);
	if (err == 0) {
		std::string_view path(url + u.field_data[UF_PATH].off, u.field_data[UF_PATH].len);
		std::cout << path << std::endl;
		if ((u.field_set & (1 << UF_QUERY)) != 0)
		{
			std::string_view param(url + u.field_data[UF_QUERY].off, u.field_data[UF_QUERY].len);
			std::cout << "param:"<< param << std::endl;
		}
		
	}


	free(parser);
	system("pause");
	return 0;
}